#ifndef __UTILS_H__
#define __UTILS_H__

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
#include "block.h"
#include "portable_semaphore.h"

typedef struct
{
    DIR *input_dir;
    char input_dir_path[PATH_MAX];
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;

void usage(char *prog_name);

int parse_args(args_t *args, int argc, char *argv[]);

#endif // __UTILS_H__