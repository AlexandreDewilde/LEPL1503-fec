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
    ret = fseek(file, 0, SEEK_SET);
    if (ret == -1) {
        fprintf(stderr, "Error with fseek : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return byte_size;
}


void get_file_info(FILE *file, file_info_t *file_info) {

    file_info->file_size =  get_file_size(file) - 24;

    uint32_t buffer[6];
    size_t readed_chunks = fread(buffer, sizeof(uint32_t), 6, file);
    if (readed_chunks != 6) {
        deal_error_reading_file(file);
    }


    file_info->seed = be32toh(buffer[0]);

    file_info->block_size = be32toh(buffer[1]);

    file_info->word_size = be32toh(buffer[2]);

    file_info->redudancy = be32toh(buffer[3]);

    file_info->message_size = be64toh(*((uint64_t*) (buffer + 4)));

    DEBUG("Seed : %d, block_size : %d, word_size : %d, redundancy : %d\n", file_info->seed, file_info->block_size, file_info->word_size, file_info->redudancy);
}


void prepare_block(block_t *block, uint32_t block_size, uint32_t global_block_size, uint32_t word_size, uint32_t redudancy) {
    block->global_block_size = global_block_size;
    block->block_size = block_size;
    block->word_size = word_size;
    block->redudancy = redudancy;
}

void make_block(uint8_t *file_data, block_t *block, uint64_t block_id) {
    block->message = file_data + (block->global_block_size + block->redudancy) * block->word_size * block_id;
}

uint32_t find_lost_words(block_t *block, bool *unknown_indexes) {
    uint32_t unknowns = 0;

    for (uint32_t i = 0; i < block->block_size;i++) {
        uint32_t count = 0;
        for (uint32_t j = 0; j < block->word_size;j++) {
            count += block->message[i*block->word_size + j];
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

    uint8_t *b_sub_line = malloc(block->word_size);

    if (!b_sub_line) {
        printf("Error to allocate memory for linear system");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < unknown; i++) {
        uint32_t temp = 0;
        for (uint32_t j = 0; j < block->block_size; j++) {
            if (unknowns_indexes[j]) {
                A[i][temp++] = coeffs[i][j];
            }
            else {
                // In case the word is not lost we have to substract to solve the system without it 
                gf_256_mul_vector_buffer(b_sub_line, block->message + j*block->word_size, coeffs[i][j], block->word_size);
                inplace_gf_256_full_add_vector(B[i], b_sub_line, block->word_size);           
            } 
        }
    }
    free(b_sub_line);  

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
            memcpy(B[i], block->message + (block->block_size + i) * block->word_size, block->word_size);
        }

        make_linear_system(A, B, unknowns_indexes, unknowns, block, coeffs);
        gf_256_gaussian_elimination(A, B, block->word_size, unknowns);

        uint32_t temp = 0;
        for (uint32_t i = 0; i < block->block_size; i++) {
            if (unknowns_indexes[i]) {
                memcpy(block->message + i*block->word_size, B[temp++], block->word_size);
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
    size_t written = fwrite(block->message, block->word_size, block->block_size, output);
    if (written != block->block_size) {
        fprintf(stderr, "Error writing to output\n");
        exit(EXIT_FAILURE);
    }
}

void write_blocks(uint8_t *message, block_t *blocks, uint32_t nb_blocks, uint64_t message_size, FILE *output) {
    if (!nb_blocks) return;
    uint8_t *current = message + blocks[0].block_size * blocks[0].word_size;
    
    for (uint32_t i = 1; i < nb_blocks; i++) {
        uint32_t to_read = blocks[i].block_size*blocks[i].word_size;
        memcpy(current, blocks[i].message, to_read);
        current += to_read;
    }
    fwrite(message, message_size, 1, output);
}


void write_last_block(block_t *block, FILE *output, uint32_t remaining, uint32_t padding) {

    size_t written = fwrite(block->message, block->word_size * remaining - padding, 1, output);
    if (written != 1) {
        fprintf(stderr, "Error writing to output\n");
        exit(EXIT_FAILURE);
    }
}
