#ifndef PROD_CONS_PROGRAM_H
#define PROD_CONS_PROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include "debug.h"
#include "block.h"
#include "producer.h"
#include "shared_buffer.h"



//initialise the global prod_index & cons_index
int prod_index = 0;
int cons_index = 0;


//Initialising the mutex which will rptect the buffer
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

//Create the buffer
block_t **buffer;

int threads_program(int argc, char *argv[]);

#endif /*PROD_CONS_PROGRAM_H */