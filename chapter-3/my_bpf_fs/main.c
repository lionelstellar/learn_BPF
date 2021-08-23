#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_manipulate.h"


int menu(){
    int choice = 0;
    while(1) {
        printf("-----select commands:-----\n"
            "1.create map\n"
            "2.update map\n"
            "3.fetch map\n"
            "4.delete map\n"
            "5.exit\n"
        );
        scanf("%d", &choice);

        switch (choice){
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
                printf("exit!\n");
                return 0;
            default:
                printf("input is invalid!\n");
                break;
        }
    }
}

int main(){
    menu();
}