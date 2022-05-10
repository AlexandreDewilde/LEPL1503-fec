#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include "debug.h"
#include "utils.h"
#include "block.h"
#include "portable_endian.h"
#include "portable_semaphore.h"

typedef struct {
    char *filename;
    FILE *file;
    FILE *output;
} file_thread;

typedef struct {
    char *filename;
    FILE *output;
    uint64_t message_size;
    block_t *blocks;
    uint64_t nb_blocks;
    uint32_t remaining;
    uint32_t padding;
    bool uncomplete_block;

} output_infos_t;

int program(int argc, char *argv[]);

#endif /*PROGRAM_H */


