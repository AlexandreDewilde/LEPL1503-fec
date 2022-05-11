#include "../headers/block.h"

int64_t get_file_size(FILE *file) {
    int ret = fseek(file, 0L, SEEK_END);
    if (ret == -1) {
        fprintf(stderr, "Error with fseek : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    int64_t byte_size = ftell(file);
    if (byte_size == -1) {
        fprintf(stderr, "Error with ftell : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    rewind(file);
    return byte_size;
}


void get_file_info(FILE *file, file_info_t *file_info) {

    file_info->file_size =  get_file_size(file) - 24;

    
    // Reads file header with all the information
    size_t readed_chunks;
    readed_chunks = fread(&(file_info->seed), sizeof(uint32_t), 1, file);
    if (readed_chunks != 1) {
        deal_error_reading_file(file);
    }
    file_info->seed = be32toh(file_info->seed);

    readed_chunks = fread(&(file_info->block_size), sizeof(uint32_t), 1, file);
    if (readed_chunks != 1) {
        deal_error_reading_file(file);
    }
    file_info->block_size = be32toh(file_info->block_size);

    readed_chunks = fread(&(file_info->word_size), sizeof(uint32_t), 1, file);
    if (readed_chunks != 1) {
        deal_error_reading_file(file);
    }
    file_info->word_size = be32toh(file_info->word_size);

    readed_chunks = fread(&(file_info->redudancy), sizeof(uint32_t), 1, file);
    if (readed_chunks != 1) {
        deal_error_reading_file(file);
    }
    file_info->redudancy = be32toh(file_info->redudancy);

    readed_chunks = fread(&(file_info->message_size), sizeof(uint64_t), 1, file);
    if (readed_chunks != 1) {
        deal_error_reading_file(file);
    }
    file_info->message_size = be64toh(file_info->message_size);

    DEBUG("Seed : %d, block_size : %d, word_size : %d, redundancy : %d\n", file_info->seed, file_info->block_size, file_info->word_size, file_info->redudancy);
}


void prepare_block(block_t *block, uint32_t block_size, uint32_t word_size, uint32_t redudancy) {

    block->block_size = block_size;
    block->word_size = word_size;
    block->redudancy = redudancy;

    // Allocate 2D array to store the message stored in this block
    block->message = malloc(block->block_size * sizeof(uint8_t*));
    if (block->message == NULL) {
        fprintf(stderr, "Failed to allocate memory for block message\n");
        exit(EXIT_FAILURE);
    }

    // Allocate 2D array to store the redudant symbols of the block
    block->redudant_symbols = malloc(block->redudancy*sizeof(uint8_t*));
    if (block->redudant_symbols == NULL) {
        fprintf(stderr, "Failed to allocate memory for redudants symbols\n");
        exit(EXIT_FAILURE);
    }

    uint8_t *temp = malloc(block->block_size * block->word_size);
    if (temp == NULL) {
        fprintf(stderr, "Failed to allocate memory for message\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < block->block_size; i++) {
        block->message[i] = temp + i * block->word_size;
    }

    temp = malloc(block->redudancy * block->word_size);
    if (temp == NULL) {
        fprintf(stderr, "Failed to allocate memory for redudancy symbols\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < block->redudancy; i++) {
        block->redudant_symbols[i] = temp + i * block->word_size;
    }
}

void free_block(block_t *block) {
    free(block->message[0]);
    free(block->message);
    free(block->redudant_symbols[0]);
    free(block->redudant_symbols);
}

void make_block(FILE *file, block_t *block) {
    size_t readed_chunks;
    // Read message from file and stored it in the block structure
    for (uint8_t i = 0; i < block->block_size; i++) {
        readed_chunks = fread(block->message[i], block->word_size, 1, file);
        if (readed_chunks != 1) {
            deal_error_reading_file(file);
        }
    }

    // Read Redudant symbols and stored it in the block structure
    for (uint8_t i = 0; i < block->redudancy; i++) {
        readed_chunks = fread(block->redudant_symbols[i], block->word_size, 1, file);
        if (readed_chunks != 1) {
            deal_error_reading_file(file);
        }
    }
}

uint32_t find_lost_words(block_t *block, bool *unknown_indexes) {
    uint32_t unknowns = 0;

    for (uint32_t i = 0; i < block->block_size;i++) {
        uint32_t count = 0;
        for (uint32_t j = 0; j < block->word_size;j++) {
            count += block->message[i][j];
        }
        // If count == 0 then all bytes equal 0 then we assume it's a lost word
        if (!count) {
            unknown_indexes[i] = true;
            unknowns++;
        }
        else {
            unknown_indexes[i] = false;
        }
    }
    DEBUG_VECTOR_BOOLEAN(unknown_indexes, block->block_size);
    return unknowns;
}

void make_linear_system(uint8_t **A, uint8_t **B, bool *unknowns_indexes, uint32_t unknown, block_t *block, uint8_t **coeffs) {

    for (uint32_t i = 0; i < unknown; i++) {
        uint32_t temp = 0;
        for (uint32_t j = 0; j < block->block_size; j++) {
            if (unknowns_indexes[j]) {
                A[i][temp++] = coeffs[i][j];
            }
            else {
                // In case the word is not lost we have to substract to solve the system without it 
                uint8_t *b_sub_line = gf_256_mul_vector(block->message[j], coeffs[i][j], block->word_size);
                inplace_gf_256_full_add_vector(B[i], b_sub_line, block->word_size);
                free(b_sub_line);               
            } 
        }
    }

    verbose_linear_system(A, B, unknown, unknown);
    DEBUG("Size :%d\n", unknown);
}

void process_block(block_t *block, uint8_t **coeffs) {
   
    bool *unknowns_indexes = malloc(block->block_size);
    if (unknowns_indexes == NULL) {
        fprintf(stderr, "Failed to allocated unknown indexes vector\n");
        exit(EXIT_FAILURE);
    }

    uint32_t unknowns = find_lost_words(block, unknowns_indexes);

    // If there is no unknown, those ops are useless
    if (unknowns) {
        uint8_t **A = malloc(unknowns * sizeof(uint8_t*));
        if (A == NULL) {
            fprintf(stderr, "Error while allocating memory processing block\n");
            exit(EXIT_FAILURE);
        }
        uint8_t **B = malloc(unknowns * sizeof(uint8_t *));
        if (B == NULL) {
            fprintf(stderr, "Error while allocating memory processing block\n");
            exit(EXIT_FAILURE);
        }

        uint8_t *temp_alloc = malloc(unknowns * unknowns);
        if (temp_alloc == NULL) {
            fprintf(stderr, "Error while allocating memory processing block\n");
            exit(EXIT_FAILURE);
        }

        for (uint32_t i = 0; i < unknowns; i++) {
            A[i] = temp_alloc + i * unknowns;
        }
        
        temp_alloc = malloc(unknowns * block->word_size);
        if (temp_alloc == NULL) {
            fprintf(stderr, "Error while allocating memory processing block\n");
            exit(EXIT_FAILURE);
        }

        for (uint32_t i = 0; i < unknowns; i++) {
            B[i] = temp_alloc + i * block->word_size;
            memcpy(B[i], block->redudant_symbols[i], block->word_size);
        }

        make_linear_system(A, B, unknowns_indexes, unknowns, block, coeffs);
        gf_256_gaussian_elimination(A, B, block->word_size, unknowns);

        uint32_t temp = 0;
        for (uint32_t i = 0; i < block->block_size; i++) {
            if (unknowns_indexes[i]) {
                memcpy(block->message[i], B[temp++], block->word_size);
            }
        }

        free(A[0]);
        free(B[0]);
        free(A);
        free(B);
    }
    free(unknowns_indexes);
}


void write_block(block_t *block, FILE *output) {
    for (uint32_t j = 0; j < block->block_size; j++) {
        size_t written = fwrite(block->message[j], block->word_size, 1, output);
        if (written != 1) {
            fprintf(stderr, "Error writing to output\n");
            exit(EXIT_FAILURE);
        }
    }
}


void write_last_block(block_t *block, FILE *output, uint32_t remaining, uint32_t padding) {
    for (uint32_t j = 0; j < remaining - 1; j++) {
        size_t written = fwrite(block->message[j], block->word_size, 1, output);
        if (written != 1) {
            fprintf(stderr, "Error writing to output\n");
            exit(EXIT_FAILURE);
        }
    }
    size_t written = fwrite(block->message[remaining - 1], block->word_size - padding, 1, output);
    if (written != 1) {
        fprintf(stderr, "Error writing to output\n");
        exit(EXIT_FAILURE);
    }
}

void free_blocks(block_t *blocks, uint32_t nb_blocks) {
    for (uint32_t i = 0; i < nb_blocks; i++) {
        free_block(&blocks[i]);
    }
    free(blocks);
}