# Chap 3 BPF映射

### 3.1 创建BPF映射

1.使用BPF系统调用

```c
int bpf(enum bpf_cmd cmd, union bpf_attr *attr, unsigned int size)
{
    return syscall(__NR_bpf, cmd, attr, size);
}


	union bpf_attr my_map = {
    	.map_type = BPF_MAP_TYPE_HASH,
    	.key_size = sizeof(int),
    	.value_size = sizeof(int),
    	.max_entries = 100,
    	.map_flags = BPF_F_NO_PREALLOC,
	};
	fd = bpf(BPF_MAP_CREATE, &my_map, sizeof(my_map));
```

map_type是映射类型，3.3中会分析。

2.使用bpf.h中对bpf系统调用的封装的LIBBPF_API——bpf_create_map()

```
LIBBPF_API int bpf_create_map(enum bpf_map_type map_type, int key_size,
			      int value_size, int max_entries, __u32 map_flags);

/* flags for BPF_MAP_CREATE command */
#define BPF_F_NO_PREALLOC	(1U << 0)
/* Instead of having one common LRU list in the
 * BPF_MAP_TYPE_LRU_[PERCPU_]HASH map, use a percpu LRU list
 * which can scale and perform better.
 * Note, the LRU nodes (including free nodes) cannot be moved
 * across different LRU lists.
 */
#define BPF_F_NO_COMMON_LRU	(1U << 1)
/* Specify numa node during map creation */
#define BPF_F_NUMA_NODE		(1U << 2)
```

用户调用

```
fd = bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(int), sizeof(int), 100, flag);
```



### 3.2 使用BPF映射

用户空间对fd进行操作，内核空间对my_map进行操作，内核使用map_data全局变量来保存BPF程序的映射信息（map_data[0].fd）。

1.更新映射元素

```
result = bpf_map_update_elem(fd, &key, &value, flag); // flag 可为BPF_ANY, BPF_NOEXIST, BPF_EXIST

/* flags for BPF_MAP_UPDATE_ELEM command */
#define BPF_ANY		0 /* create new element or update existing */
#define BPF_NOEXIST	1 /* create new element if it didn't exist */
#define BPF_EXIST	2 /* update existing element */
```

2.读取映射元素

```
result = bpf_map_lookup_elem(fd, &key, &value);
```

3.删除映射元素

```
result = bpf_map_delete_elem(fd, &key);
```

4.迭代映射元素

```
while(bpf_map_get_next_key(fd, &lookup_key, &next_key) == 0){
        bpf_map_lookup_elem(fd, &next_key, &value);
        printf("The next key in the map is: '%d', value is: '%d'\n", next_key, value);
        lookup_key = next_key;
}
```

5.查找并删除映射元素

```
result = bpf_map_lookup_and_delete_elem(fd, &key, &value);
```

6.并发访问元素

内核中有2个函数bpf_spin_lock和bpf_spin_unlock和自旋锁一起使用，例如给count元素添加信号：

```
struct concurrent_element {
	struct bpf_spin_lock semaphore;
	int count;
}
```

将结构体保存在BPF映射中：

```
struct bpf_map_def SEC("maps") concurrrent_map = {
	.type				= BPF_MAP_TYPE_HASH,
	.key_size		= sizeof(int),
	.value_size = sizeof(struct concurrent_element),
	.max_entries= 100,
};
```



### 3.3 BPF映射类型

<img width="326" alt="bpf_map_type" src="https://user-images.githubusercontent.com/23394091/130574252-b8e2dab9-fe13-43ba-be0e-c441cb836965.png">

BPF_MAP_TYPE_HASH，哈希表映射，可使用任意大小的key和value，按需分配和释放，查找迅速。

BPF_MAP_TYPE_ARRAY，创建时在内存预分配空间并将value置零，预分配key为0~99，元素不能删除。

BPF_MAP_TYPE_PROG_ARRAY，保存对bpf程序的引用，键和值都是4字节。

(待更新...)



### 3.4 BPF虚拟文件系统

BPF虚拟文件系统默认目录是/sys/fs/bpf，可通过命令挂载：

`mount -t bpf /sys/fs/bpf /sys/fs/bpf`

BPF对象和程序都将以fd形式持久化在BPF虚拟化文件系统中，只能通过bpf()来操作。

eg:

```c
/*持久化bpf map*/
fd = bpf_map_create(...)		//创建bpf map
added = bpf_map_update_elem(fd, &key, &value, BPF_ANY)	//将key、value更新到map中
pinned = bpf_obj_pin(fd, "/sys/fs/bpf/my_array")	//将bpf map持久化到文件中
  
/*访问bpf map*/
fd = bpf_obj_get("/sys/fs/bpf/my_array");	//获取bpf对象
result = bpf_map_lookup_elem(fd, &key, &value);		//根据key来查找value
```

有了BPF虚拟文件系统，BPF对象可以持久化并且被不同的进程之间共享。













