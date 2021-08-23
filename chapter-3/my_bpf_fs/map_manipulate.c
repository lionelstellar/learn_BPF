#include <errno.h>
#include <linux/bpf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "map_manipulate.h"
#include "bpf.h"

// 动态创建bpf对象文件
int get_all_files(char bpf_obj_list[FILE_NUM][FILE_NAME_SIZE]){
    struct dirent *ptr;
    DIR *dir;
    dir = opendir(bpf_fs);
    char tmp[FILE_NAME_SIZE] = {0};
    int  cnt = 0;
    printf("********* bpf obj files *********\n");
    while((ptr = readdir(dir)) != NULL){
        strcpy(tmp, ptr->d_name);
        if(tmp[0] != '.'){
            strcpy(bpf_obj_list[cnt], tmp);
            cnt++;
            printf("%d:%s\n", cnt, tmp);
        }

    }
    if (cnt == 0) printf("None!\n");
    printf("*********     over      *********\n");

    closedir(dir);
    return cnt;
}

int get_file_path(int flag, char *ret_path){
    
    char bpf_obj_list[FILE_NUM][FILE_NAME_SIZE] = {0};

    int cnt = get_all_files(bpf_obj_list);
   
    char file_path[FILE_NAME_SIZE] = {0};
    strcat(ret_path, bpf_fs);
    int choice = 0;

    if(flag == CREATE) {
        printf("Input the create file_name(len <= FILE_NAME_SIZE):\n");
        scanf("%s", file_path);
        strcat(ret_path, file_path);
        
    } else if ((flag == UPDATE || flag == FETCH || flag == DELETE) && cnt > 0) {
        while(1) {
            printf("Input the number of the file you wanna update:\n");
            scanf("%d", &choice);
            if (choice <= 0 || choice > cnt ) {
                printf("Invalid input!\n");
            } else {
                strcat(ret_path, bpf_obj_list[choice - 1]);
                break;
            }
        }
    } else {
        printf("input is invalid!\n");
        ret_path = NULL;
        return GET_FILE_FAILED;
    }

    printf("ret_path is %s\n", ret_path);
    return GET_FILE_SUCCESS;
}

void get_key(int *key){
	printf("Input Key:\n");
    scanf("%d", key);
}

void get_value(int *value){
	printf("Input Value:\n");
    scanf("%d", value);
}

int create_map(){
	int key, value, fd, added, pinned;
    char ret_path[20 + FILE_NAME_SIZE] = {0};
	fd = bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(int), sizeof(int), 100, 0);

  	if (fd < 0) {
    	printf("Failed to create map: %d (%s)\n", fd, strerror(errno));
    	return -1;
  	}

	get_file_path(CREATE, ret_path);
    get_key(&key);
    get_value(&value);

	added = bpf_map_update_elem(fd, &key, &value, BPF_ANY);
	if (added < 0) {
    	printf("Failed to update map: %d (%s)\n", added, strerror(errno));
    	return -1;
	}

  	// 4.4版本新加的，属于持久化eBPF。 有了这个，eBPF-maps和eBPF程序可以放入/sys/fs/bpf
  	pinned = bpf_obj_pin(fd, ret_path);
  	if (pinned < 0) {
    	printf("Failed to pin map to the file system: %d (%s)\n", pinned,
        strerror(errno));
    	return -1;
  	}
	return 0;
}

int update_map(){
    
	int key, value, fd, old_value, result;
    char ret_path[20 + FILE_NAME_SIZE] = {0};
    int status = get_file_path(UPDATE, ret_path);
    if(status != GET_FILE_SUCCESS) {
        printf("fail to update!\n");
        return -1;
    }

    get_key(&key);
    get_value(&value);

  	fd = bpf_obj_get(ret_path);
    
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

// int fetch_map(){
// 	int fd, key, value, result;
//     char ret_path[20 + FILE_NAME_SIZE] = {0};
//     get_file_path(FETCH, ret_path);
//     get_key(&key);


//   	fd = bpf_obj_get(ret_path);
//   	if (fd < 0) {
//     	printf("Failed to fetch the map: %d (%s)\n", fd, strerror(errno));
//     	return -1;
//   	}

//   	result = bpf_map_lookup_elem(fd, &key, &value);
//   	if (result < 0) {
//     	printf("Failed to read value from the map: %d (%s)\n", result,
//         strerror(errno));
//     	return -1;
//   	} 

//   	printf("Value read from the map: '%d'\n", value);
// 	return 0;
// }

int fetch_map(){
	int fd, key, value, result;
    char ret_path[20 + FILE_NAME_SIZE] = {0};
    int status = get_file_path(FETCH, ret_path);
    if(status != GET_FILE_SUCCESS) {
        printf("fail to fetch!\n");
        return -1;
    }


    get_key(&key);
    fd = bpf_obj_get(ret_path);
    printf("path is %s\n", ret_path);
  	if (fd < 0) {
    	printf("Failed to fetch the map: %d (%s)\n", fd, strerror(errno));
    	return -1;
  	}

  	result = bpf_map_lookup_elem(fd, &key, &value);
    printf("result is %d\n", result);
  	if (result < 0) {
    	printf("Failed to read value from the map: %d (%s)\n", result,
        strerror(errno));
    	return -1;
  	} 

  	printf("Value read from the map: '%d'\n", value);
	return 0;
}


int delete_map(){
    char ret_path[20 + FILE_NAME_SIZE] = {0};
    int status = get_file_path(DELETE, ret_path);
    if(status != GET_FILE_SUCCESS) {
        printf("fail to delete!\n");
        return -1;
    }

    return 0;
}