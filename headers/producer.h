#ifndef PRODUCER_H
#define PRODUCER_H

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
#include "shared_buffer.h"
#include "prod_cons_program.h"


void producer(char *filename, FILE *file, buffer_info **buffer, int nb_of_file);

#endif /*PRODUCER_H */