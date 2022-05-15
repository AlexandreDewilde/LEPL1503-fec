#ifndef _DEBUG_H
#define _DEBUG_H

#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h> 
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>


/**
*
*
* @param start pointer to the variable that keeps the starting time
*/
void DEBUG_start_time(struct timeval *start);

/**
*
*
* @param end pointer to the variable that keeps the ending time
*/
void DEBUG_end_time(struct timeval *end);

/**
*
*
* @param the pointer to the variables that keep the starting time and the ending time
* @return: the difference between the starting time and the ending time
*/
double DEBUG_get_delta_time(struct timeval start, struct timeval end);


/*
    a print time DEBUG function 
*/
void DEBUG_PRINT_TIME_USED(double used_time);

void verbose_matrix(uint8_t **A, uint32_t size, uint32_t b_size);
void verbose_linear_system(uint8_t **A, uint8_t **B, uint32_t size, uint32_t b_size);

/*
    a general DEBUG function 
*/
void DEBUG(const char *restrict format, ...);
/*
    DEBUG_VECTOR prints a given vector
*/
void DEBUG_VECTOR(uint8_t *vector, uint32_t vector_size);
void DEBUG_VECTOR_BOOLEAN(bool *vector, uint32_t vector_size);
void ACTIVATE_DEBUG();
void deal_error_reading_file(FILE *file);
#endif /* _DEBUG_H */