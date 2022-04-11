#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h> 
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "debug.h"

static int SIGNAL = 0;

void DEBUG(const char *restrict format, ...){
    if (SIGNAL){
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}
//Ce DEBUG imprime le vecteur donné
void DEBUG_VECTOR(uint8_t *vector, uint32_t vector_size){
    fprintf(stderr, "This is the vector: ");
    for(uint32_t i = 0; i < vector_size; i++){
        fprintf(stderr,"%d", *vector);
        vector++;
    }
    fprintf(stderr, "\n");
}

void ACTIVATE_DEBUG(){
    SIGNAL = 1;
}