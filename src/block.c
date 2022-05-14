#include "../headers/block.h"

int math_ceil(double nb) {
    int nb_int = (int) nb;
    if (nb == (double) nb_int) {
        return nb_int;
    }
    return nb_int + 1;
}

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


void get_file_info_from_buffer(uint8_t *buffer, file_info_t *file_info) {


    file_info->seed = be32toh(*((uint32_t*)buffer));

    file_info->block_size = be32toh(*((uint32_t*) (buffer + sizeof(uint32_t))));

    file_info->word_size = be32toh(*((uint32_t*) (buffer + sizeof(uint32_t) * 2)));

    file_info->redudancy = be32toh(*((uint32_t*) (buffer + sizeof(uint32_t) * 3)));

    file_info->message_size = be64toh(*((uint64_t*) (buffer + 4 * sizeof(uint32_t))));

    DEBUG("Seed : %d, block_size : %d, word_size : %d, redundancy : %d\n", file_info->seed, file_info->block_size, file_info->word_size, file_info->redudancy);
}

uint32_t find_lost_words(block_t *block, bool *unknown_indexes) {
    uint32_t unknowns = 0;

    for (uint32_t i = 0; i < block->block_size;i++) {
        bool count = 0;
        for (uint32_t j = 0; j < block->word_size;j++) {
            if (block->message[i*block->word_size + j]) {
                count = true;
                break;
            }
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

void process_block(block_t *block, uint8_t **coeffs, bool *unknowns_indexes, uint32_t redudancy) {
   
    if (unknowns_indexes == NULL) {
        fprintf(stderr, "Failed to allocated unknown indexes vector\n");
        exit(EXIT_FAILURE);
    }

    uint32_t unknowns = find_lost_words(block, unknowns_indexes);

    if (unknowns > redudancy) {
        fprintf(stderr, "Too much unknowns symbols\n");
        return;
    }

    // If there is no unknown, those ops are useless
    if (unknowns) {
        uint8_t **A = malloc(2 * unknowns * sizeof(uint8_t*) + unknowns * unknowns + unknowns * block->word_size);
        if (A == NULL) {
            fprintf(stderr, "Error while allocating memory processing block\n");
            exit(EXIT_FAILURE);
        }
        uint8_t **B = A + unknowns;

        uint8_t *temp_alloc = (uint8_t *) (A + 2 * unknowns);

        for (uint32_t i = 0; i < unknowns; i++) {
            A[i] = temp_alloc + i * unknowns;
        }
        
        temp_alloc += unknowns * unknowns;
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
        free(A);
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
    size_t written = fwrite(message, message_size, 1, output);
    if (written != 1) {
        fprintf(stderr, "Error writing block to output\n");
        exit(EXIT_FAILURE);
    }
}



void parse_file(output_infos_t *output_infos, file_thread_t *file_thread) {
    if (!file_thread->filename) {
        memset(output_infos, 0, sizeof(output_infos_t));
        return;
    }

    file_info_t file_info;
    file_info.file_size =  get_file_size(file_thread->file);

    uint8_t *file_data = malloc(file_info.file_size);
    if (file_data == NULL) {
        fprintf(stderr, "Error allocating memory for file\n");
        exit(EXIT_FAILURE);
    }
    uint8_t *binary_data = file_data + 24;

    size_t readed = fread(file_data, 1, file_info.file_size, file_thread->file);
    if (readed != file_info.file_size) {
        fprintf(stderr, "Error reading file\n");
        exit(EXIT_FAILURE);
    }

    get_file_info_from_buffer(file_data, &file_info);
    
    uint8_t **coeffs = gen_coefs(file_info.seed, file_info.block_size, file_info.redudancy);
    verbose_matrix(coeffs, file_info.redudancy, file_info.block_size);
    
    uint64_t step = file_info.word_size * (file_info.block_size + file_info.redudancy);
    uint64_t nb_blocks = math_ceil((file_info.file_size - 24) / (double) step);

    block_t *blocks = malloc(nb_blocks*sizeof(block_t));
    if (blocks == NULL) {
        fprintf(stderr, "Error allocating memory for block\n");
        exit(EXIT_FAILURE);
    }

    bool uncomplete_block = file_info.message_size != nb_blocks * file_info.block_size * file_info.word_size; 

    
    bool *unknowns_indexes = malloc(file_info.block_size);

    for (uint64_t i = 0; i < nb_blocks - uncomplete_block; i++) {
        blocks[i].block_size = file_info.block_size;
        blocks[i].word_size = file_info.word_size;
        blocks[i].message = binary_data + (file_info.block_size + file_info.redudancy) * file_info.word_size * i;
        process_block(&blocks[i], coeffs, unknowns_indexes, file_info.redudancy);
    }

    uint32_t remaining = ( (file_info.file_size - 24 - (nb_blocks-uncomplete_block) * step) / file_info.word_size) - file_info.redudancy;
    
    if (uncomplete_block) {
        blocks[nb_blocks-1].block_size = remaining;
        blocks[nb_blocks-1].word_size = file_info.word_size;
        blocks[nb_blocks-1].message = binary_data + (file_info.block_size + file_info.redudancy) * file_info.word_size * (nb_blocks-1);
        process_block(&blocks[nb_blocks-1], coeffs, unknowns_indexes, file_info.redudancy);
    }

    free(unknowns_indexes);

    output_infos->file_data = file_data;
    output_infos->file_size = file_info.file_size;
    output_infos->message_size = file_info.message_size;
    output_infos->blocks = blocks;
    output_infos->nb_blocks = nb_blocks;
    output_infos->remaining = remaining;
    output_infos->uncomplete_block = uncomplete_block;
    output_infos->filename = file_thread->filename;

    free(coeffs);
}


void write_to_file(output_infos_t *output_infos, FILE *output_stream) {
    uint32_t filename_length = htobe32(strlen(output_infos->filename));
    size_t written = fwrite(&filename_length, sizeof(uint32_t), 1, output_stream);
    if (written != 1) {
        fprintf(stderr, "Error writing to output the length of filename");
        exit(EXIT_FAILURE);
    }

    uint64_t message_size_be = htobe64(output_infos->message_size);
    written = fwrite(&message_size_be, sizeof(uint64_t), 1, output_stream);
    if (written != 1) {
        fprintf(stderr, "Error writing to output the message size\n");
        exit(EXIT_FAILURE);
    }
    written = fwrite(output_infos->filename, strlen(output_infos->filename), 1, output_stream);
    if (written != 1) {
        fprintf(stderr, "Error writing to output the filename\n");
        exit(EXIT_FAILURE);
    }
    if (output_infos->nb_blocks > 0) {
        write_blocks(output_infos->blocks[0].message, output_infos->blocks, output_infos->nb_blocks, output_infos->message_size, output_stream);
    }

    free(output_infos->file_data);
    free(output_infos->blocks);
    free(output_infos->filename);
}
