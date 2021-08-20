#include <errno.h>
#include <linux/bpf.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "bpf.h"
#include "bpf/bpf_helpers.h"


// bpf文件系统
static const char *file_path1 = "/sys/fs/bpf/my_array1";
static const char *file_path2 = "/sys/fs/bpf/my_array2";

int update_map(const char *file_path, int value){
	int fd, value, result;

  	fd = bpf_obj_get(file_path);
  	if (fd < 0) {
    	printf("Failed to fetch the map: %d (%s)\n", fd, strerror(errno));
    	return -1;
  	}


int main(int argc, char **argv) {
	update_map(file_path1, 12345);
	update_map(file_path2, 23456);
  
	return 0;
}