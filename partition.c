#include "partition.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool open_existing_partition(Fat_Head* fat_head, char* partition_name){
    FILE* partition_ptr = fopen(partition_name,"rb");
    if(partition_ptr == NULL){
        puts("no partition found, try again");
        return false;
    }
    fread(fat_head,1,sizeof(Fat_Head),partition_ptr);
    fclose(partition_ptr);
    printf("succes: found and read in partition: %s\n",partition_name);
    return true; 
}

bool create_new_partition(Fat_Head* fat_head, char* partition_name){
    FILE* partition_ptr = fopen(partition_name,"wb");
    if(partition_ptr == NULL){
        puts("partition creation failed, try again");
        return false;
    }
    strcpy(fat_head->boot_sector.partition_name,partition_name);
    fat_head->boot_sector.block_count = BLOCK_COUNT;
    fat_head->boot_sector.block_size = BLOCK_SIZE;
    fwrite(fat_head,1,sizeof(Fat_Head), partition_ptr);
    void* data_sector_memory = calloc(BLOCK_COUNT,BLOCK_SIZE); // reserving memory for data sector
    fwrite(data_sector_memory,1,BLOCK_COUNT*BLOCK_SIZE,partition_ptr);
    printf("succes: you created new partition: %s\n",partition_name);
    fclose(partition_ptr);
    return true; 
}

