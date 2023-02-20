#include "delete.h"
#include <string.h>
#include <stdio.h>
void rm(Fat_Head* fat_head,char* file_name){
 FILE* partition_ptr = fopen(fat_head->boot_sector.partition_name,"rb+");
 if(partition_ptr == NULL){
   puts("no partition found, try again");
   return;
 }
 int root_entry_num = file_root_entry_num(fat_head,file_name);
 unsigned int previous_block;
 for(unsigned int current_block = fat_head->root_directory.entries[root_entry_num].first_block; current_block != EOF;){
    previous_block = current_block;
    current_block = fat_head->fat.table[current_block];
    fat_head->fat.table[previous_block] = 0;
 }
 memset(fat_head->root_directory.entries[root_entry_num].file_name,'\0',32);
 fwrite(fat_head,1,sizeof(Fat_Head),partition_ptr); // write fat_head to partition
 fclose(partition_ptr);
}
