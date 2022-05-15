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

/*
* This structrure contains all arguments given by the user
*/
typedef struct
{
    DIR *input_dir;
    char input_dir_path[PATH_MAX];
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;

/*
* This function prints informations about the program
* @param : prog_name : the directory of the program
*/
void usage(char *prog_name);


/*
* This function treats all arguments given by the user
* @param : args : a pointer of all important arguments
* @param : argc : the number of arguments given by the user
* @param : args : the arguments given by the user
*/
int parse_args(args_t *args, int argc, char *argv[]);

#endif // __UTILS_H__