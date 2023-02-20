#include "add.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void add(char* file_name,Fat_Head* fat_head){ // add file to partition
    FILE* partition_ptr = fopen(fat_head->boot_sector.partition_name,"rb+");
    if(partition_ptr == NULL){
        printf("\n%s\n",fat_head->boot_sector.partition_name);
        puts("no partition found, try again");
        return;
    }
    FILE* file_ptr = fopen(file_name,"rb+");
    if(file_ptr == NULL){
        puts("no file found, try again");
        return;
    }
    void* file_one_block = malloc(BLOCK_SIZE);
    if(file_one_block == NULL){
        puts("no memory reserved");
        return;
    }
    unsigned int root_entry_num = 0;
    unsigned int fat_first_free_block = 0;
    unsigned int file_block_cnt = file_fat_block_count(file_name);
    printf("FILE BLOCKS: %d\n",file_block_cnt);
    for(; root_entry_num < MAX_ROOT_DIRECTORY_ENTRIES; ++root_entry_num){
        if(fat_head->root_directory.entries[root_entry_num].file_name[0] == '\0'){ // fill root_directory with file stats
            strcpy(fat_head->root_directory.entries[root_entry_num].file_name,file_name);
            fat_head->root_directory.entries[root_entry_num].file_size = file_size(file_name);
            fat_head->root_directory.entries[root_entry_num].is_dir = false;
            for(; fat_first_free_block < BLOCK_COUNT; ++fat_first_free_block){
                if(file_block_cnt == 0 && fat_head->fat.table[fat_first_free_block] == 0){
                    fat_head->root_directory.entries[root_entry_num].first_block = EOF;
                    break; 
                }
                if(fat_head->fat.table[fat_first_free_block] == 0){
                    fat_head->root_directory.entries[root_entry_num].first_block = fat_first_free_block;
                    break; 
                }
            }
            break;
        }
    }
    for(unsigned int fat_next_free_block = fat_head->fat.table[fat_first_free_block]+1,current_block = fat_first_free_block; // fill fat table blocks for given file
    file_block_cnt != 0; ++fat_next_free_block){
        if(fat_head->fat.table[fat_next_free_block] == 0 && file_block_cnt > 1){ 
            fat_head->fat.table[current_block] = fat_next_free_block; current_block = fat_next_free_block; 
            --file_block_cnt;
        }
        if(fat_head->fat.table[fat_next_free_block] == 0 && file_block_cnt == 1){ 
            fat_head->fat.table[current_block] = EOF; // EOF(-1) file end
            --file_block_cnt;
        }
    }
    fwrite(fat_head,1,sizeof(Fat_Head),partition_ptr); // write fat_head to partition
    for(unsigned int current_block = fat_first_free_block; current_block != EOF;  // write file blocks to partition
    current_block = fat_head->fat.table[current_block]){
        fread(file_one_block,1,BLOCK_SIZE,file_ptr);
        fseek(partition_ptr,sizeof(fat_head)+BLOCK_SIZE*current_block,SEEK_SET);
        fwrite(file_one_block,1,BLOCK_SIZE,partition_ptr);
    }
    fclose(partition_ptr);
    fclose(file_ptr);
    free(file_one_block);
    printf("writen file: %s to partition: %s\n",file_name,fat_head->boot_sector.partition_name);
}