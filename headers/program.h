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
#include <pthread.h>
#include "debug.h"
#include "utils.h"
#include "block.h"
#include "portable_endian.h"
#include "portable_semaphore.h"

/*
* The function runs the whole program and is called by the main function in main.c
* @param : argc : the number of arguments given by the user
* @param : argv: the arguments given by the user
*/
int program(int argc, char *argv[]);

#endif /*PROGRAM_H */


