#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../headers/tools.h"
#include "../../headers/system.h"
#include "../../headers/program.h"


/**
*
*
* @param
* @return: 
*/

void start_time(struct timeval *start) {
    // From https://stackoverflow.com/questions/10192903/time-in-milliseconds-in-c
    gettimeofday(start, NULL);
}

/**
*
*
* @param
* @return: 
*/

void end_time(struct timeval *end) {
    gettimeofday(end, NULL);    
}


/**
*
*
* @param
* @return: 
*/

double get_delta_time(struct timeval start, struct timeval end) {
    return (double) (end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);
}

/**
*
*
* @param
* @return: 
*/

void test_gf_256_gaussian_elimination_random(uint32_t matrix_size, uint32_t solutions_size, uint32_t repeat) {
    struct timeval stop, start;
    double average_delta_time = 0;

    printf("Start testing Gaussian Elimination On random matrix\n");

    uint8_t** A = generate_zero_matrix(matrix_size, matrix_size);
    uint8_t** B = generate_zero_matrix(matrix_size, solutions_size);

    for (uint32_t i = 0; i < repeat; i++) {
        
        fill_matrix_random(A, matrix_size, matrix_size);
        fill_matrix_random(B, matrix_size, solutions_size);

        start_time(&start);
        gf_256_gaussian_elimination(A, B, solutions_size, matrix_size);
        end_time(&stop);

        average_delta_time += get_delta_time(start, stop);
    }

    free(A[0]);
    free(B[0]);
    free(A);
    free(B);
    average_delta_time /= repeat;

    printf("Ended Testing speed of gaussian elimination on random matrices of size %dx%d and solution size of %dx%d, the average time : %lf on %d sample(s)\n", matrix_size, matrix_size, matrix_size, solutions_size, average_delta_time, repeat);
    
}

/**
*
*
* @param
* @return: 
*/

void test_gf_256_gaussian_elimination(int repeat) {
    struct timeval stop, start;
    double average_delta_time = 0;
    printf("Start testing Gaussian Elimination\n");
    uint8_t A[3][3] = { {1,2,3}, {5,5,6}, {11,8,9} };
    uint8_t b[3][3] = { {0,1,2}, {1,1,1}, {2,2,2} };

    // Convert to double pointer **
    uint8_t *A_double_pointers[3] = {A[0], A[1], A[2]};
    uint8_t *b_double_pointers[3] = {b[0], b[1], b[2]};

    uint8_t** current_A = generate_zero_matrix(3, 3);
    uint8_t** current_b = generate_zero_matrix(3, 3);
      
    for (int i = 0; i < repeat; i++) {
        fill_matrix(A_double_pointers, current_A, 3, 3);
        fill_matrix(b_double_pointers, current_b, 3, 3);

        start_time(&start);
        gf_256_gaussian_elimination(current_A, current_b, 3, 3);
        end_time(&stop);

        average_delta_time += get_delta_time(start, stop);
    }
    free(current_A[0]);
    free(current_b[0]);
    free(current_A);
    free(current_b);

    average_delta_time /= repeat;
    printf("Ended Testing speed of gaussian elimination average time : %lf\n", average_delta_time);
}

void test_program() {
    struct timeval stop, start;
    start_time(&start);
    char *argv[] = {"./fec", "samples", "-f", "test.txt"};
    program(4, argv);
    end_time(&stop);
    printf("Time to execute: %lf\n", get_delta_time(start, stop));
}


int main()
{
    test_program();
    test_gf_256_gaussian_elimination(1000);
    test_gf_256_gaussian_elimination_random(100, 100, 100);    
}