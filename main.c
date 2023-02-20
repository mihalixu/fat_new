#include "fat.h"
#include "partition.h"
#include "add.h"
#include "put.h"
#include "delete.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int main(){
    Fat_Head fat_head = {0}; 
    PWD pwd[32]; 
    unsigned int pwd_count = 0;
    char input_str[128];
    bool partition_exist = false;
    int input_partition;
    while(!partition_exist){
        char partition_name[64];
        char buffer[16];
        printf("Enter 1 to create new_partition, 2 to use existing partition, 3 to exit program: ");
        fgets(buffer,16,stdin);
        sscanf(buffer,"%d",&input_partition);
        if(input_partition == 1){ 
            printf("Enter partition name: ");
            fgets(partition_name,64,stdin);
            partition_name[strcspn(partition_name, "\n")] = '\0';
            if(partition_exist = create_new_partition(&fat_head,partition_name)){ // create_new_partition returns bool for succes
                stpcpy(pwd[pwd_count].name,partition_name);
                ++pwd_count;
            } 
        }
        else if(input_partition == 2){
            printf("Enter partition name: ");
            fgets(partition_name,64,stdin);
            partition_name[strcspn(partition_name, "\n")] = '\0';
            if(partition_exist = open_existing_partition(&fat_head,partition_name)){ // open_existing_partition returns bool for succes
                stpcpy(pwd[pwd_count].name,partition_name);
                ++pwd_count;
            }             
        }
        else if(input_partition == 3){
            exit(1);
        }
    }
    cli(pwd,pwd_count);
    while(fgets(input_str,128,stdin)){
        input_str[strcspn(input_str, "\n")] = '\0'; // remove new line char
        char* substr = strtok(input_str," "); // split string
        while(substr != NULL){
            if(!strcmp(substr,"add")){
                char* file_name = strtok(NULL, " ");
                add(file_name,&fat_head);
            }
            if(!(strcmp(substr,"show"))){
                puts("show");
                show(&fat_head);
            }
            if(!(strcmp(substr,"dir"))){
                dir(&fat_head);
            }
            if(!(strcmp(substr,"rm"))){
                char* file_name = strtok(NULL, " ");
                rm(&fat_head,file_name);
            }
            if(!(strcmp(substr,"put"))){
                char* old_file_name = strtok(NULL, " ");
                char* new_file_name = strtok(NULL, " ");
                put(&fat_head,old_file_name,new_file_name);
            }
            if(!(strcmp(substr,"exit"))){
                puts("exit program");
                exit(0);
            }
            substr = strtok(NULL," "); // get next comand string
        }
        cli(pwd,pwd_count);
    }
    return 0;
}







