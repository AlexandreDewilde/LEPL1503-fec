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

void verbose_matrix(uint8_t **A, uint32_t size, uint32_t b_size) {
    if (SIGNAL){
        fprintf(stderr, ">> Système linéaire\n");
        for (uint32_t i = 0; i < size;i++) {
            fprintf(stderr, "[");
            for (uint32_t j = 0; j < b_size; j++) {
                if (j + 1 == size)
                    fprintf(stderr, "%d", A[i][j]);
                else
                    fprintf(stderr, "%d ", A[i][j]);
            }
            fprintf(stderr, "]");
            //DEBUG("Still writing the matrix A\n");
        }
        fprintf(stderr, "\n");
    }
    //DEBUG("Done writing the matrix A\n");
}


void verbose_linear_system(uint8_t **A, uint8_t **B, uint32_t size, uint32_t b_size) {
    if (SIGNAL){
        fprintf(stderr, ">> Système linéaire\n");
        for (uint32_t i = 0; i < size;i++) {
            fprintf(stderr, "[");
            for (uint32_t j = 0; j < size; j++) {
                if (j + 1 == size)
                    fprintf(stderr, "%d", A[i][j]);
                else
                    fprintf(stderr, "%d ", A[i][j]);
            }
            fprintf(stderr, "]\t[");
            for (uint32_t j = 0; j < b_size; j++) {
                if (j + 1 == b_size)
                    fprintf(stderr, "%d", B[i][j]);
                else
                    fprintf(stderr, "%d ", B[i][j]);
            }
            fprintf(stderr, "]\n");
        }
        fprintf(stderr, "\n");
    }
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
            fprintf(stderr, "%d ", *vector);
            vector++;
        }
        fprintf(stderr, "\n");
    }  
}

void DEBUG_VECTOR_BOOLEAN(bool *vector, uint32_t vector_size) {
    if (SIGNAL){
        for(uint32_t i = 0; i < vector_size; i++){
            fprintf(stderr, "%d ", *vector);
            vector++;
        }
        fprintf(stderr, "\n");
    }
}

void ACTIVATE_DEBUG(){
    SIGNAL = 1;
}