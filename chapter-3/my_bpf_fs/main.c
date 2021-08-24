#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_manipulate.h"


int menu(){
    int choice = 0;
    while(1) {
        printf("\n\n\n\n\n-----select commands:-----\n"
            "0.exit\n"
            "1.create map\n"
            "2.update map\n"
            "3.fetch map\n"
            "4.delete map\n"
            "5.iterate map\n"     
            "6.lookup and delete map\n"     
        );
        scanf("%d", &choice);

        switch (choice){
            case 0:
                printf("exit!\n");
                return 0;
            case 1:
                create_map();
                break;
            case 2:
                update_map();
                break;
            case 3:
                fetch_map();
                break;
            case 4:
                delete_map();
                break;
            case 5:
                iterate_map();
                break;
            case 6:
                lookup_and_delete_map();
                break;
            default:
                printf("[+] input is invalid!\n");
                break;
        }
    }
}

int main(){
    menu();
}