#ifndef FAT_H
#define FAT_H
#define BLOCK_SIZE 4096
#define BLOCK_COUNT 1024
#define MAX_ROOT_DIRECTORY_ENTRIES 32
#define MAX_SUBDIR_ENTRIES 32
#include <stdbool.h>

typedef struct{
    char partition_name[32];
    unsigned int block_count;
    unsigned int block_size;
} Boot_Sector;

typedef struct {
    unsigned int table[BLOCK_COUNT];
} FAT;

typedef struct {
    char file_name[32];
    bool is_dir;
    unsigned int first_block;
    unsigned int file_size;  
} Root_Directory_Entrie;

typedef struct{
    Root_Directory_Entrie entries[MAX_ROOT_DIRECTORY_ENTRIES];
} Root_Directory;

typedef struct{
    Boot_Sector boot_sector;
    FAT fat;
    Root_Directory root_directory;
} Fat_Head;

typedef struct{
    char name[32]; 
} PWD;

void cli(PWD pwd[], unsigned int size);
void show(Fat_Head* fat_head);
void dir(Fat_Head* fat_head);
unsigned int file_size(char* file_name);
unsigned int file_fat_block_count(char* file_name);
unsigned int file_root_entry_num(Fat_Head* fat_head, char* file_name);
#endif