#ifndef BUFFER_INFO_H
#define BUFFER_INFO_H

#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h> 
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "../headers/block.h"
#include "../headers/block.h"
#include <pthread.h>
#include <semaphore.h>


typedef struct
{
    char *filename;
    uint32_t filename_length;
    uint64_t message_size;
    bool uncomplete_block;
    uint32_t remaining;
    uint32_t padding;
    uint64_t nb_blocks;
    block_t *blocks;
    int nb_of_file;
    int64_t current_pos;
} buffer_info;

#endif /*BUFFER_INFO_H*/