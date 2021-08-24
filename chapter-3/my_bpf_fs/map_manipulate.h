// bpf文件系统
static const char *file_path1 = "/sys/fs/bpf/my_array1";
static const char *file_path2 = "/sys/fs/bpf/my_array2";
static char *bpf_fs = "/sys/fs/bpf/";

#define FILE_NAME_SIZE 30
#define FILE_NUM       1024

enum {
    CREATE = 0,
    UPDATE = 1,
    FETCH  = 2,
    DELETE = 3,
    ITERATE= 4,
    LOOKUP_DELETE = 5,
};

enum {
    GET_FILE_SUCCESS = 0,
    GET_FILE_FAILED  = 1,
};

int create_map();
int update_map();
int fetch_map();
int delete_map();
int iterate_map();
int lookup_and_delete_map();