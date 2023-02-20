#ifndef PARTITION_H
#define PARTITION_H
#include "fat.h"

bool open_existing_partition(Fat_Head* fat_head, char* partition_name);
bool create_new_partition(Fat_Head* fat_head, char* partition_name);

#endif