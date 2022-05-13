#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>

#include "portable_endian.h"
#include "system.h"
#include "debug.h"


typedef struct {
    uint32_t block_size;
    uint8_t *message;
} block_t;

typedef struct {
    uint32_t seed, block_size, word_size, redudancy;
    uint64_t file_size, message_size;

} file_info_t;

typedef struct {
    char *filename;
    FILE *file;
} file_thread_t;

typedef struct {
    uint8_t *file_data;
    char *filename;
    uint64_t file_size;
    uint64_t message_size;
    uint32_t word_size;
    block_t *blocks;
    uint64_t nb_blocks;
    uint32_t remaining;
    bool uncomplete_block;

} output_infos_t;


void get_file_info(FILE *file, file_info_t *file_info);

void get_file_info_from_buffer(uint8_t *buffer, file_info_t *file_info);

void make_block(uint8_t *file_data, block_t *block, uint64_t file_position);

void free_blocks(block_t *blocks, uint32_t nb_blocks);


uint32_t find_lost_words(block_t *block, bool *unknown_indexes, uint32_t word_size);

/**
 *
 * Build a linear system Ax=b from block argument
 * @param A: Empty matrix already allocated, that the function will modify
 * @param B: Empty matrix already allocated, that the function will modify
 * @param unknowns_indexes: Number of unknowns ub the linear system
 * @param block: the block that the linear system will be build from
 * @param coeffs: the coeffs to generate the A matrix of the linear system
 */
void make_linear_system(uint8_t **A, uint8_t **B, bool *unknowns_indexes, uint32_t unknown, uint32_t word_size, block_t *block, uint8_t **coeffs);

void process_block(block_t *block, uint8_t **coeffs, bool *unknowns_indexes, uint32_t word_size);

void write_block(block_t *block, FILE *output);

void write_last_block(block_t *block, FILE *output, uint32_t remaining, uint32_t padding);

void write_blocks(uint8_t *message, block_t *blocks, uint32_t nb_blocks, uint32_t word_size, uint64_t message_size, FILE *output);

void parse_file(output_infos_t *output_infos, file_thread_t *file_thread);

void write_to_file(output_infos_t *output_infos, FILE *output_stream);

#endif /* BLOCK_H */