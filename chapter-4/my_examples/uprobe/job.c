#include <stdio.h>

void do_job1(void){
    usleep(1);
    // printf("job1 done, consume 1us!\n");
}

void do_job2(void){
    sleep(1);
    // printf("job2 done, consume 1s!\n");
}

int main(void){
    do_job1();
    do_job2();
    return 0;
}

