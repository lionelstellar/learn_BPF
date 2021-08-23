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

int update_map(const char *file_path, int key, int value){
	int fd, old_value, result;

  	fd = bpf_obj_get(file_path);
    
  	if (fd < 0) {
    	printf("Failed to fetch the map: %d (%s)\n", fd, strerror(errno));
    	return -1;
  	}

    result = bpf_map_lookup_elem(fd, &key, &old_value);
  	if (result < 0) {
    	printf("Failed to read value from the map: %d (%s)\n", result,
        strerror(errno));
    	return -1;
  	}
    printf("old is %d\n", old_value);
    
    result = bpf_map_update_elem(fd, &key, &value, BPF_ANY);
    if (result == 0) {
        printf("The value of key %d is changed from %d to %d\n", key, old_value, value);
    } else {
        printf("Failed to update map (%s)\n", strerror(errno));
    }
    
    return 0;
}


int main(int argc, char **argv) {
	update_map(file_path1, 1, 12345);
	update_map(file_path2, 2, 23456);
  
	return 0;
}