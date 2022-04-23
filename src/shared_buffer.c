#include "../headers/shared_buffer.h"


buffer_info **create_shared_buffer(int size){
    buffer_info **buffer = (buffer_info **) malloc(size * sizeof(buffer_info *));
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0 , size);  // buffer vide
    sem_init(&full, 0 , 0);   // buffer vide

    return buffer;

}

void produce_in_shared_buffer(buffer_info *buffer_index, buffer_info *data){
    buffer_index = &(*data);
    
                                           
}

void consume_in_shared_buffer(buffer_info **buffer, FILE *output) {
    consumer(buffer, output);

}