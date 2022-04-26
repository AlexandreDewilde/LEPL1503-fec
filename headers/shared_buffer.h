#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#include "buffer_info.h"
#include "consumer.h"

typedef struct 
{
    buffer_info **buffer;
    FILE *output;
} consumer_args;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;


buffer_info **create_shared_buffer(int size);
void produce_in_shared_buffer(buffer_info **buffer, int index, buffer_info *data);
void consume_in_shared_buffer(void * args);

#endif /*SHARED_BUFFER_H*/