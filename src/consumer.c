#include "../headers/consumer.h"
#include "../headers/shared_buffer.h"
#include "../headers/prod_cons_program.h"
#include "../headers/producer.h"

void free_block(block_t *block) {
    free(block->message[0]);
    free(block->message);
    free(block->redudant_symbols[0]);
    free(block->redudant_symbols);
}

void write_block(block_t *block, FILE *output) {
    for (uint32_t j = 0; j < block->block_size; j++) {
        size_t written = fwrite(block->message[j], block->word_size, 1, output);
        if (written != 1) {
            DEBUG("Error writing to output\n");
            exit(EXIT_FAILURE);
        }
    }
}


void write_last_block(block_t *block, FILE *output, uint32_t remaining, uint32_t padding) {
    for (uint32_t j = 0; j < remaining - 1; j++) {
        size_t written = fwrite(block->message[j], block->word_size, 1, output);
        if (written != 1) {
            DEBUG("Error writing to output\n");
            exit(EXIT_FAILURE);
        }
    }
    size_t written = fwrite(block->message[remaining - 1], block->word_size - padding, 1, output);
    if (written != 1) {
        DEBUG("Error writing to output\n");
        exit(EXIT_FAILURE);
    }
}

void free_blocks(block_t *blocks, uint32_t nb_blocks) {
    for (uint32_t i = 0; i < nb_blocks; i++) {
        free_block(&blocks[i]);
    }
    free(blocks);
}

void consumer(buffer_info **buffer, FILE *output){
    sem_wait(&full); // attente d'une place remplie
    pthread_mutex_lock(&mutex);

    buffer_info *current_index = buffer[cons_index];
    
    size_t written = fwrite(&(current_index->filename_length), sizeof(uint32_t), 1, output);
    if (written != 1) {
        DEBUG("Error writing to output the length of filename");
        exit(EXIT_FAILURE);
    }
    written = fwrite(&(current_index->message_size), sizeof(uint64_t), 1, output);
    if (written != 1) {
        DEBUG("Error writing to output the message size\n");
        exit(EXIT_FAILURE);
    }
    written = fwrite(current_index->filename, strlen(current_index->filename), 1, output);
    if (written != 1) {
        printf("Error writing to output the filename\n");
        exit(EXIT_FAILURE);
    }
    for (uint64_t i = 0; i < current_index->nb_blocks - current_index->uncomplete_block; i++) {
        write_block(&(current_index->blocks[i]), output);
    }
    if (current_index->current_pos == -1) {
        DEBUG("Error with ftell : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (current_index->uncomplete_block) {
        write_last_block(&(current_index->blocks[current_index->nb_blocks-1]), output, current_index->remaining, current_index->padding);
    }

    free_blocks(current_index->blocks, current_index->nb_blocks);

    //Increment the index of the buffer
    if (cons_index < current_index->nb_of_file){
        cons_index += 1;
    }else{
        cons_index = 0;
    }
    
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    

}