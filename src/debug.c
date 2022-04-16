#include "../headers/debug.h"

static int SIGNAL = 0;

void deal_error_reading_file(FILE *file) {
    if (ferror(file)) {
        DEBUG("This error occured reading the file : %s\n", strerror(errno));
    }
    else {
        DEBUG("File ended, check that file is formated correctly\n");
    }
    exit(EXIT_FAILURE);
}

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
    if (SIGNAL){
        fprintf(stderr, "This is the vector: ");
        for(uint32_t i = 0; i < vector_size; i++){
            fprintf(stderr,"%d", *vector);
            vector++;
        }
        fprintf(stderr, "\n");
    }
    
}

void ACTIVATE_DEBUG(){
    SIGNAL = 1;
}