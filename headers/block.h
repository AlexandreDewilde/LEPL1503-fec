#ifndef BLOCK_H
#define BLOCK_H
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>
#include "system.h"
#include "message.h"
#include "portable_endian.h"

typedef struct {
    uint32_t block_size, word_size, redudancy;
    uint8_t **message;
    uint8_t **redudant_symbols;
} block_t;

typedef struct {
    uint32_t seed, block_size, word_size, redudancy;
    uint64_t file_size, message_size;

} file_info_t;



void get_file_info(FILE *file, file_info_t *file_info);

void prepare_block();
void make_block();
void free_blocks();

uint32_t find_lost_words();

void make_linear_system();
void process_block();
void write_block();
void write_last_block();
message_t *blocks_to_message_t(block_t *blocks, uint32_t nb_blocks, bool uncomplete_block, uint32_t block_size, uint32_t word_size, uint32_t remaining, uint32_t padding);
void parse_file();

#endif /* BLOCK_H */