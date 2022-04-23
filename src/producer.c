#include "../headers/producer.h"
#include "../headers/shared_buffer.h"
#include "../headers/prod_cons_program.h"
#include "../headers/consumer.h"



/*
 A producer is a thread that calculates the matrix
 and add it in the buffer
*/
void producer(char *filename, FILE *file, buffer_info **buffer, int nb_of_file){
    buffer_info *info_cons = (buffer_info *) malloc(sizeof(buffer_info)); // We make sure the information is kept and passed on to the consumer 
    info_cons->filename = filename;
    info_cons->nb_of_file = nb_of_file;
    file_info_t file_info;
    get_file_info(file, &file_info);
    
    uint8_t **coeffs = gen_coefs(file_info.seed, file_info.block_size, file_info.word_size);
    verbose_matrix(coeffs, file_info.word_size, file_info.block_size);
    
    uint64_t step = file_info.word_size * (file_info.block_size + file_info.redudancy);
    uint64_t nb_blocks = ceil(file_info.file_size / (double) step);
    info_cons->nb_blocks = nb_blocks;

    block_t *blocks = malloc(nb_blocks*sizeof(block_t));
    if (blocks == NULL) {
        DEBUG("Error allocating memory for block\n");
        exit(EXIT_FAILURE);
    }

    bool uncomplete_block = file_info.message_size != nb_blocks * file_info.block_size * file_info.word_size; 
    info_cons->uncomplete_block = uncomplete_block;

    uint32_t filename_length = htobe32(strlen(filename));
    info_cons->filename_length = filename_length;
    
    uint64_t message_size = htobe64(file_info.message_size);
    info_cons->message_size = message_size;
    
    for (uint64_t i = 0; i < nb_blocks - uncomplete_block; i++) {
        prepare_block(&blocks[i], file_info.block_size, file_info.word_size, file_info.redudancy);
        make_block(file, &blocks[i]);
        process_block(&blocks[i], coeffs);
    }

    int64_t current_pos = ftell(file);
    info_cons->current_pos = current_pos;
    if (current_pos == -1) {
        DEBUG("Error with ftell : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    uint32_t remaining = ( (file_info.file_size + 24 - current_pos) / file_info.word_size) - file_info.redudancy;
    info_cons->remaining = remaining;

    uint32_t padding = (file_info.block_size * file_info.word_size * (nb_blocks - 1)) + remaining * file_info.word_size - file_info.message_size;
    info_cons->padding = padding;
    if (uncomplete_block) {
        prepare_block(&blocks[nb_blocks-1], remaining, file_info.word_size, file_info.redudancy);
        make_block(file, &blocks[nb_blocks-1]);
        process_block(&blocks[nb_blocks-1], coeffs);
    }
    // Adding the blocks in the consumer_struct
    info_cons->blocks = blocks;
    // Adding the consumer_info struct in the buffer

    sem_wait(&empty); // attente d'une place libre
    pthread_mutex_lock(&mutex);
     // section critique
    produce_in_shared_buffer(buffer[prod_index], info_cons);

    //Increment the index of the buffer
    if (prod_index < nb_of_file){
        prod_index += 1;
    }else{
        prod_index = 0;
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&full); // il y a une place remplie en plus

    // Freeing everything
    free(coeffs[0]);
    free(coeffs);
    
    

}