#ifndef CONSUMER_H
#define CONSUMER_H

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
#include "buffer_info.h"
#include "prod_cons_program.h"


void consumer(buffer_info **buffer, FILE *output);

#endif /*CONSUMER_H */