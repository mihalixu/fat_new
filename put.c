#include "put.h"
#include <stdio.h>
#include <stdlib.h>

void put(Fat_Head* fat_head,char* old_file_name,char* new_file_name){
    FILE* partition_ptr = fopen(fat_head->boot_sector.partition_name,"rb");
    if(partition_ptr == NULL){
        printf("\n%s\n",fat_head->boot_sector.partition_name);
        puts("no partition found, try again");
        return;
    }
    FILE* file_ptr = fopen(new_file_name,"wb");
    if(file_ptr == NULL){
        puts("no file found, try again");
        return;
    }
    void* file_one_block = malloc(BLOCK_SIZE);
    if(file_one_block == NULL){
        puts("no memory reserved");
        return;
    }
    unsigned int file_entry = file_root_entry_num(fat_head,old_file_name);
    for(unsigned int current_block = fat_head->root_directory.entries[file_entry].first_block; current_block != EOF;  // write file blocks to partition
    current_block = fat_head->fat.table[current_block]){
        fseek(partition_ptr,sizeof(fat_head)+BLOCK_SIZE*current_block,SEEK_SET);
        fread(file_one_block,1,BLOCK_SIZE,partition_ptr);
        fwrite(file_one_block,1,BLOCK_SIZE,file_ptr);
    }
    fclose(partition_ptr);
    fclose(file_ptr);
    free(file_one_block);
}