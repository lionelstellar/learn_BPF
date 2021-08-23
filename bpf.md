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

2.使用bpf.h中对bpf系统调用的封装的LIBBPF_API——bpf_create_map()

```
fd = bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(int), sizeof(int), 100, 0);
```

### 3.2 使用BPF映射

用户空间对fd进行操作，内核空间对my_map进行操作，内核使用map_data全局变量来保存BPF程序的映射信息（map_data[0].fd）。

1.更新映射元素

```
result = bpf_map_lookup_elem(fd, &key, &value, flag); // flag 可为BPF_ANY, BPF_NOEXIST, BPF_EXIST

```

2.读取映射元素

```
result = bpf_map_lookup_elem(fd, &key, &value);
```

3.删除映射元素

```
result = bpf_map_delete_elem(fd, &key);
```





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













