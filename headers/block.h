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
    uint32_t block_size, word_size;
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
    block_t *blocks;
    uint64_t nb_blocks;
    uint32_t remaining;
    bool uncomplete_block;

} output_infos_t;


/**
 *
 * Help to have file info 
 * @param file: the file we want to get informations 
 * @param file_info: Structure 
 */
void get_file_info(FILE *file, file_info_t *file_info);

/**
 *
 * Help to have file info from a buffer
 * @param buffer: the buffer  we want to get informations 
 * @param file_info: Structure 
 */
void get_file_info_from_buffer(uint8_t *buffer, file_info_t *file_info);


/**
 *
 * Help to have find lost word in our file
 * @param block: the block we want to find if there is lost words 
 * @param unknown_indexes: A list of  boolean tell us if yes or no there is a lost symbol  in the block
 */
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
void make_linear_system(uint8_t **A, uint8_t **B, uint8_t *b_sub_line, bool *unknowns_indexes, uint32_t unknown, block_t *block, uint8_t **coeffs);

/**
 *
 * Prepare Block to allow writing 
 * @param block: The block that will have part of message 
 * @param unknowns_indexes: a list of bollean
 * @param redudancy: the block that the linear system will be build from
 * @param coeffs: the coeffs to generate that help us to solve the linear_ststem
 */
void process_block(block_t *block, uint8_t **coeffs, uint8_t *b_sub_line, bool *unknowns_indexes, uint32_t redudancy);

/**
 *
 * Write message in output file 
 * @param message: The message to write in the output file
 * @param message_size : The size of the message 
 * @param nb_blocks: Number of blocks
 * @param blocks: the block that will contain message 
 * @param output : the file in witch we will write bloc
 */
void write_blocks(uint8_t *message, block_t *blocks, uint32_t nb_blocks, uint64_t message_size, FILE *output);

/**
 *
 * Parse our files.
 * @param Output_infos: Structures 
 * @param file_thread:  Structure 
 */
void parse_file(output_infos_t *output_infos, file_thread_t *file_thread);

/**
 *
 * Help to write in the strucutre.
 * @param output_infos: Structure that will have our OutputScream file info
 * @param Butput_stream : File that we want to keep informations
 */
void write_to_file(output_infos_t *output_infos, FILE *output_stream);

void get_file_info_from_buffer(uint8_t *buffer, file_info_t *file_info);

int64_t get_file_size(FILE *file);

#endif /* BLOCK_H */