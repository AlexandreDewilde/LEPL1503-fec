#include "../headers/debug.h"

static int SIGNAL = 0;


/**
*
*
* @param the pointer to the variable that keeps the starting time
* @return: 
*/
void DEBUG_start_time(struct timeval *start) {
    if (SIGNAL){
    // From https://stackoverflow.com/questions/10192903/time-in-milliseconds-in-c
    gettimeofday(start, NULL);
    }
}

/**
*
*
* @param the pointer to the variable that keeps the ending time
* @return: 
*/

void DEBUG_end_time(struct timeval *end) {
    if (SIGNAL){
        gettimeofday(end, NULL);
    }
}


/**
*
*
* @param the pointer to the variables that keep the starting time and the ending time
* @return: the difference between the starting time and the ending time
*/

double DEBUG_get_delta_time(struct timeval *start, struct timeval *end) {
    if (SIGNAL){
        return (double) (end->tv_usec - start->tv_usec) / 1000000 + (double)(end->tv_sec - start->tv_sec);
    }
    return 0;
}

void DEBUG_PRINT_TIME_USED(double used_time){
    if(SIGNAL){
        fprintf(stderr, "\nThe total time used by the program is : %f\n ", used_time);
    }
}


void deal_error_reading_file(FILE *file) {
    if (ferror(file)) {

        fprintf(stderr, "\nThis error occured reading the file : %s\n", strerror(errno));
    }
    else {
        fprintf(stderr, "\nFile ended, check that file is formated correctly\n");
    }
    exit(EXIT_FAILURE);
}

void verbose_matrix(uint8_t **A, uint32_t size, uint32_t b_size) {
    if (SIGNAL){
        fprintf(stderr, "\nHere is the matrix : \n");
        for (uint32_t i = 0; i < size;i++) {
            fprintf(stderr, "[");
            for (uint32_t j = 0; j < b_size; j++) {
                if (j + 1 == size)
                    fprintf(stderr, "%d", A[i][j]);
                else
                    fprintf(stderr, "%d ", A[i][j]);
            }
            fprintf(stderr, "]");
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n");
    }
}


void verbose_linear_system(uint8_t **A, uint8_t **B, uint32_t size, uint32_t b_size) {
    if (SIGNAL){
        fprintf(stderr, "\nHere is A and B of the linear system equation: \n");
        for (uint32_t i = 0; i < size;i++) {
            fprintf(stderr, "A = \n");
            fprintf(stderr, "[");
            for (uint32_t j = 0; j < size; j++) {
                if (j + 1 == size)
                    fprintf(stderr, "%d", A[i][j]);
                else
                    fprintf(stderr, "%d ", A[i][j]);
            }
            fprintf(stderr, "]");
            fprintf(stderr, "]\n");
            fprintf(stderr, "B = \n");
            fprintf(stderr, "[");
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

/*
    a general DEBUG function 
*/
void DEBUG(const char *restrict format, ...){
    if (SIGNAL){
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

/*
    DEBUG_VECTOR prints a given vector
*/
void DEBUG_VECTOR(uint8_t *vector, uint32_t vector_size){
    if (SIGNAL){
        fprintf(stderr, "\nThis is the vector: ");
        fprintf(stderr, "[");
        for(uint32_t i = 0; i < vector_size; i++){
            fprintf(stderr, "%d ", *vector);
            vector++;
        }
        fprintf(stderr, "]");
        fprintf(stderr, "\n");
    }  
}

void DEBUG_VECTOR_BOOLEAN(bool *vector, uint32_t vector_size) {
    if (SIGNAL){
        fprintf(stderr, "\nThis is the boolean vector: ");
        fprintf(stderr, "[");
        for(uint32_t i = 0; i < vector_size; i++){
            if (*vector == true){
                fprintf(stderr, "%s ", "true");
            }else{
                fprintf(stderr, "%s ", "false");
            }
            vector++;
        }
        fprintf(stderr, "]");
        fprintf(stderr, "\n");
    }
}

void ACTIVATE_DEBUG(){
    SIGNAL = 1;
}