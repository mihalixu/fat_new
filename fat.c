#include "fat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cli(PWD pwd[], unsigned int size){
    printf("~");
    for(int i = 0; i < size; i++){
        printf("/%s",pwd[i].name);
    }
    printf("$ ");
}

void show(Fat_Head* fat_head){
    for(int i = 0; i < fat_head->boot_sector.block_count; i++){
        printf("%d %d\n",i,fat_head->fat.table[i]);
    }
}

void dir(Fat_Head* fat_head){
    printf("\nfile_name                is_dir    first_block         file_size\n----------------------------------------------------------------------\n");
    for(int i = 0; fat_head->root_directory.entries[i].file_name[0] != '\0';i++){
        printf("%s                   %d        %d                %d\n",fat_head->root_directory.entries[i].file_name,fat_head->root_directory.entries[i].is_dir,
        fat_head->root_directory.entries[i].first_block, fat_head->root_directory.entries[i].file_size);
    }
    printf("\n");
}

unsigned int file_size(char* file_name){
    FILE* file_ptr = fopen(file_name, "rb");
	fseek(file_ptr, 0, SEEK_END); // set file cursor to file end
	int file_size = ftell(file_ptr);
	fclose(file_ptr);
	return file_size;   
}

unsigned int file_fat_block_count(char* file_name){
    unsigned int  block_count = file_size(file_name) / BLOCK_SIZE;
	if (file_size(file_name) % BLOCK_SIZE != 0){ ++block_count; }
	return block_count;
}

unsigned int file_root_entry_num(Fat_Head* fat_head, char* file_name){
    for(int i = 0; i < MAX_ROOT_DIRECTORY_ENTRIES; i++){
        if(!(strcmp(fat_head->root_directory.entries[i].file_name,file_name))){
            printf("file_root_entry_num: %d\n",i);
            return i;
        }
    }
    puts("no file_root_entry_num found, try again");
    return -1;
}