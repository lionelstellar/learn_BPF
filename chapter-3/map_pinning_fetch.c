#include <errno.h>
#include <linux/bpf.h>
#include <stdio.h>
#include <string.h>
#include "bpf.h"

static const char *file_path1 = "/sys/fs/bpf/my_array1";
static const char *file_path2 = "/sys/fs/bpf/my_array2";

int fetch(const char* file_path, int key){
	int fd, value, result;

  	fd = bpf_obj_get(file_path);
  	if (fd < 0) {
    	printf("Failed to fetch the map: %d (%s)\n", fd, strerror(errno));
    	return -1;
  	}

  	result = bpf_map_lookup_elem(fd, &key, &value);
  	if (result < 0) {
    	printf("Failed to read value from the map: %d (%s)\n", result,
        strerror(errno));
    	return -1;
  	} 

  	printf("Value read from the map: '%d'\n", value);
	return 0;
}

int main(int argc, char **argv) {
	fetch(file_path1, 1);
	// fetch(file_path1, 2);
	fetch(file_path2, 2);

  	return 0;
}
