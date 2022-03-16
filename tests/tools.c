#include "headers/tools.h"

uint8_t **deep_copy(uint8_t **arr, uint32_t x, uint32_t y) {
    uint8_t **new_arr = malloc(x*sizeof(uint8_t*));
    for (uint32_t i = 0; i < x; i++) {
        new_arr[i] = malloc(y);
        memcpy(new_arr[i], arr[i], y);
    }
    return new_arr;
}

void fill_matrix(uint8_t **src, uint8_t **dest, uint32_t x, uint32_t y) {
    for (uint32_t i = 0; i < x; i++) {
        for (uint32_t j = 0; j < y; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

uint8_t** generate_zero_matrix(int x, int y) {
    uint8_t **matrix = malloc(x*sizeof(uint8_t*));
    for (int i = 0; i < x; i++) {
        matrix[i] = malloc(y);
        for (int j = 0; j < y; j++) matrix[i][j] = 0;
    }
    return matrix;
}


uint8_t **random_matrix(uint32_t n, uint32_t m) {
    uint8_t** arr = malloc(n*sizeof(uint8_t*));
    for (uint32_t i = 0; i < n; i++) {
        arr[i] = malloc(m);
        for (uint32_t j = 0; j < m; j++) {
            arr[i][j] = (uint8_t) (rand() % 256);
        }
    }
    return arr;
}

void fill_matrix_random(uint8_t **matrix, uint32_t n, uint32_t m) {
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < m; j++) {
            matrix[i][j] = (uint8_t) (rand() % 256);
        }
    }
}