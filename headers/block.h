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

/*
* This structrure contains informations about a block
*/
typedef struct {
    uint32_t block_size, word_size;
    uint8_t *message;
} block_t;

/*
* This structrure contains informations about a file
*/
typedef struct {
    uint32_t seed, block_size, word_size, redudancy;
    uint64_t file_size, message_size;

} file_info_t;

/*
* This structrure contains informations about a file thread
* file : the file use by the thread
* filename : the filename of the used file
*/
typedef struct {
    char *filename;
    FILE *file;
} file_thread_t;

/*
* This structrure contains informations about the output file
*/
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
 * This function gets the information of the given file 
 * @param file: the file we want to get informations 
 * @param file_info: Structure 
 */
void get_file_info(FILE *file, file_info_t *file_info);

/**
 *
 * This function finds lost word in our file
 * @param block: the block we want to find if there is lost words 
 * @param unknown_indexes: A list of  boolean tell us if yes or no there is a lost symbol  in the block
 */
uint32_t find_lost_words(block_t *block, bool *unknown_indexes);

/**
 *
 * This function builds a linear system Ax=b from block argument
 * @param A: Empty matrix already allocated, that the function will modify
 * @param B: Empty matrix already allocated, that the function will modify
 * @param unknowns_indexes: Number of unknowns ub the linear system
 * @param block: the block that the linear system will be build from
 * @param coeffs: the coeffs to generate the A matrix of the linear system
 */
void make_linear_system(uint8_t **A, uint8_t **B, uint8_t *b_sub_line, bool *unknowns_indexes, uint32_t unknown, block_t *block, uint8_t **coeffs);

/**
 *
 * This function prepares the block to allow writing 
 * @param block: The block that will have part of message 
 * @param unknowns_indexes: a list of bollean
 * @param redudancy: the block that the linear system will be build from
 * @param coeffs: the coeffs to generate that help us to solve the linear_ststem
 */
void process_block(block_t *block, uint8_t **coeffs, uint8_t *b_sub_line, bool *unknowns_indexes, uint32_t redudancy);

/**
 *
 * This function writes message in output file 
 * @param message: The message to write in the output file
 * @param message_size : The size of the message 
 * @param nb_blocks: Number of blocks
 * @param blocks: the block that will contain message 
 * @param output : the file in witch we will write bloc
 */
void write_blocks(uint8_t *message, block_t *blocks, uint32_t nb_blocks, uint64_t message_size, FILE *output);

/**
 *
 * This function treats the information give by the the structure file_thread (containing the file information)
 * and puts the result in the structure output_infos
 * @param output_infos: a pointer to the structure where the function keeps the result 
 * @param file_thread:  a pointer to the structure keeping the information to be treated by the function
 */
void parse_file(output_infos_t *output_infos, file_thread_t *file_thread);

/**
 *
 * This function writes in the output file the given information.
 * @param output_infos: a pointer of the file that have information to write in the output file
 * @param output_stream : a pointer of the file that we want to keep informations in (write in)
 */
void write_to_file(output_infos_t *output_infos, FILE *output_stream);

/**
 *
 * This function gets the file information from a gievn buffer
 * @param buffer: the buffer  we want to get informations from
 * @param file_info: a pointer to the structure containing the file info 
 */
void get_file_info_from_buffer(uint8_t *buffer, file_info_t *file_info);


/**
 *
 * This function gets the file information from a gievn buffer
 * @param buffer: the buffer  we want to get informations from
 * @param file_info: a pointer to the structure containing the file info 
 */
void get_file_info_from_buffer(uint8_t *buffer, file_info_t *file_info);

/* This function is duplicated********************************************
 * Get file informations comming to the buffer.
 * @param Buffer: Buffer that will have our file info
 * @param file_info : the File structure in with we want to keep informations
 */
void get_file_info_from_buffer(uint8_t *buffer, file_info_t *file_info);


/**
 *
 * This function returns the size of the given file 
 * @param file: the file we want want to size
 */
int64_t get_file_size(FILE *file);

#endif /* BLOCK_H */