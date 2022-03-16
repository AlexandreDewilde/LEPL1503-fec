#include <stdio.h>
#include <sys/time.h>
#include "../../headers/system.h"

void start_time(struct timeval *start) {
    // From https://stackoverflow.com/questions/10192903/time-in-milliseconds-in-c
    gettimeofday(start, NULL);
}

void end_time(struct timeval *end) {
    gettimeofday(end, NULL);    
}

double get_delta_time(struct timeval start, struct timeval end) {
    return (double) (end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);
}

uint8_t **deep_copy(uint8_t **arr, uint32_t x, uint32_t y) {
    uint8_t **new_arr = malloc(x*sizeof(uint8_t*));
    for (uint32_t i = 0; i < x; i++) {
        new_arr[i] = malloc(y);
        memcpy(new_arr[i], arr[i], y);
    }
    return new_arr;
}

void test_gf_256_gaussian_elimination(int repeat) {
    struct timeval stop, start;
    double average_delta_time = 0;
    printf("Start testing Gaussian Elimination\n");
    uint8_t A[3][3] = { {1,2,3}, {5,5,6}, {11,8,9} };
    uint8_t b[3][3] = { {0,1,2}, {1,1,1}, {2,2,2} };

    // Convert to double pointer **
    uint8_t *A_double_pointers[3] = {A[0], A[1], A[2]};
    uint8_t *b_double_pointers[3] = {b[0], b[1], b[2]};

      
    for (int i = 0; i < repeat; i++) {
        uint8_t** current_A = deep_copy(A_double_pointers, 3, 3);
        uint8_t** current_B = deep_copy(b_double_pointers, 3, 3);
        start_time(&start);
        gf_256_gaussian_elimination(current_A, current_B, 3, 3);
        end_time(&stop);
        free(current_A);
        free(current_B);
        average_delta_time += get_delta_time(start, stop);
    }
    average_delta_time /= repeat;
    printf("Ended Testing speed of gaussian elimination average time : %lf\n", average_delta_time);
}

int main()
{
    test_gf_256_gaussian_elimination(1);
    
}