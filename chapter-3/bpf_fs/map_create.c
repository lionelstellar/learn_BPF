#include <errno.h>
#include <linux/bpf.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "bpf.h"

// bpf文件系统
static const char *file_path1 = "/sys/fs/bpf/my_array1";
static const char *file_path2 = "/sys/fs/bpf/my_array2";

int bpf(enum bpf_cmd cmd, union bpf_attr *attr, unsigned int size)
{
    return syscall(__NR_bpf, cmd, attr, size);
}

int create_map1(){
	int key, value, fd, added, pinned;
	union bpf_attr my_map = {
    	.map_type = BPF_MAP_TYPE_HASH,
    	.key_size = sizeof(int),
    	.value_size = sizeof(int),
    	.max_entries = 100,
    	.map_flags = BPF_F_NO_PREALLOC,
	};
	fd = bpf(BPF_MAP_CREATE, &my_map, sizeof(my_map));

  	if (fd < 0) {
    	printf("Failed to create map: %d (%s)\n", fd, strerror(errno));
    	return -1;
  	}

	key = 1, value = 1234;
	added = bpf_map_update_elem(fd, &key, &value, BPF_ANY);
	if (added < 0) {
    	printf("Failed to update map: %d (%s)\n", added, strerror(errno));
    	return -1;
	}

  	// 4.4版本新加的，属于持久化eBPF。 有了这个，eBPF-maps和eBPF程序可以放入/sys/fs/bpf
  	pinned = bpf_obj_pin(fd, file_path1);
  	if (pinned < 0) {
    	printf("Failed to pin map to the file system: %d (%s)\n", pinned,
        strerror(errno));
    	return -1;
  	}
	return 0;
}


int create_map2(){
	int key, value, fd, added, pinned;

	fd = bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(int), sizeof(int), 100, 0);

  	if (fd < 0) {
    	printf("Failed to create map: %d (%s)\n", fd, strerror(errno));
    	return -1;
  	}

	key = 2, value = 2345;
	added = bpf_map_update_elem(fd, &key, &value, BPF_ANY);
	if (added < 0) {
    	printf("Failed to update map: %d (%s)\n", added, strerror(errno));
    	return -1;
	}

  	// 4.4版本新加的，属于持久化eBPF。 有了这个，eBPF-maps和eBPF程序可以放入/sys/fs/bpf
  	pinned = bpf_obj_pin(fd, file_path2);
  	if (pinned < 0) {
    	printf("Failed to pin map to the file system: %d (%s)\n", pinned,
        strerror(errno));
    	return -1;
  	}
	return 0;
}



int main(int argc, char **argv) {
	create_map1();
	create_map2();
  
	return 0;
}
