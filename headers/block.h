#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

#include "portable_endian.h"
#include "system.h"
#include "debug.h"


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

void prepare_block(block_t *block, uint32_t block_size, uint32_t word_size, uint32_t redudancy);

void make_block(FILE *file, block_t *block);

void free_blocks(block_t *blocks, uint32_t nb_blocks);

uint32_t find_lost_words(block_t *block, bool *unknown_indexes);

/**
 *
 * Build a linear system Ax=b from block argument
 * @param A: Empty matrix already allocated, that the function will modify
 * @param B: Empty matrix already allocated, that the function will modify
 * @param unknowns_indexes: Number of unknowns ub the linear system
 * @param block: the block that the linear system will be build from
 * @param coeffs: the coeffs to generate the A matrix of the linear system
 */
void make_linear_system(uint8_t **A, uint8_t **B, bool *unknowns_indexes, uint32_t unknown, block_t *block, uint8_t **coeffs);

void process_block(block_t *block, uint8_t **coeffs);

void write_block(block_t *block, FILE *output);

void write_last_block(block_t *block, FILE *output, uint32_t remaining, uint32_t padding);

void parse_file(char *filename, FILE *file, FILE *output);


#endif /* BLOCK_H */