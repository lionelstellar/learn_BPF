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

int delete_map(const char *file_path, int key){
	int fd, result;

  	fd = bpf_obj_get(file_path);
    
  	if (fd < 0) {
    	printf("Failed to fetch the map: %d (%s)\n", fd, strerror(errno));
    	return -1;
  	}

    result = bpf_map_delete_elem(fd, &key);
  	if (result < 0) {
    	printf("Failed to delete key from the map: %d (%s)\n", result,
        strerror(errno));
    	return -1;
  	}
    
    printf("delete OK!\n"); 
    return 0;
}


int main(int argc, char **argv) {
	delete_map(file_path1, 100);
	// update_map(file_path2, 2, 23456);
  
	return 0;
}