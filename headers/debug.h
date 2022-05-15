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

/*
* The function gives the starting time / current time when called
* This function is used when debugging the program
* @param : start : a pointer where the function will store the result
*/
void DEBUG_start_time(struct timeval *start);

/*
* The function gives the ending time / current time when called
* This function is used when debugging the program
* @param : end : a pointer where the function will store the result
*/
void DEBUG_end_time(struct timeval *end);

/*
* The function gives the delta time / the difference between 2 given times
* This function is used when debugging the program
* @param : start : a variable containing the starting time
* @param : end : a variable containing the ending time
* @return : the difference between the start and the end time
*/
double DEBUG_get_delta_time(struct timeval start, struct timeval end);

/*
* The function prints the given time
* This function is used when debugging the program
* @param : used_time : a variable containing the time to print
*/
void DEBUG_PRINT_TIME_USED(double used_time);

/*
* The function prints the given matrix
* This function is used when debugging the program
* @param : A : the matrix to print
* @param : size : number of rows of the matrix
* @param : b_size : number of columns of the matrix
*/
void verbose_matrix(uint8_t **A, uint32_t size, uint32_t b_size);

/*
* The function prints the given matrices of the equation Ax = B
* This function is used when debugging the program
* @param : A : the matrix to print
* @param : B : the matrix to print
* @param : size : number of rows of the matrix
* @param : b_size : number of columns of the matrix
*/
void verbose_linear_system(uint8_t **A, uint8_t **B, uint32_t size, uint32_t b_size);

/*
* The function prints the given parameters
* This function is used when debugging the program
* @param : format : the format to be printed in
* @param : ... : parameters to print
*/
void DEBUG(const char *restrict format, ...);

/*
* The function prints the given vector
* This function is used when debugging the program
* @param : vector : the vector to be printed
* @param : vector_size: the size of the given vector
*/
void DEBUG_VECTOR(uint8_t *vector, uint32_t vector_size);

/*
* The function prints the given vector (containing boolean values)
* This function is used when debugging the program
* @param : vector : the vector to be printed
* @param : vector_size: the size of the given vector
*/
void DEBUG_VECTOR_BOOLEAN(bool *vector, uint32_t vector_size);

/*
* The function activates the SIGNAL when the verbose mode is enable
*/
void ACTIVATE_DEBUG();

/*
* The function prints the error occured from the given file
* This function is used when debugging the program
* @param : file : the file used to check errors
*/
void deal_error_reading_file(FILE *file);
#endif /* _DEBUG_H */