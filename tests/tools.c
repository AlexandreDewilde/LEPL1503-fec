#include "headers/tools.h"


int compare_2Darray(uint8_t** arr1, uint8_t** arr2, uint32_t size_x, uint32_t size_y) {
    for (uint32_t i = 0; i < size_x; i++) {
        for (uint32_t j = 0; j < size_y; j++) {
            if (arr1[i][j] != arr2[i][j]) return -1;
        }
    }
    return 0;
}

uint8_t* generate_random_vector(uint32_t size) {
    uint8_t *vector = malloc(size);
    for (uint32_t i = 0; i < size; i++) {
        vector[i] = (uint8_t) (rand() % 256);
    }
    return vector;
}

void fill_vector_random(uint8_t *vector, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        vector[i] = (uint8_t) (rand() % 256);
    }
}


uint8_t **deep_copy(uint8_t **arr, uint32_t x, uint32_t y) {
    uint8_t **matrix = malloc(x * sizeof(uint8_t *));
    uint8_t *temp = malloc(x * y);
    for (uint32_t i = 0; i < x; i++) {
        matrix[i] = temp + (i * y);
        memcpy(matrix[i], arr[i], y);
    }
    return matrix;
}

void fill_matrix(uint8_t **src, uint8_t **dest, uint32_t x, uint32_t y) {
    for (uint32_t i = 0; i < x; i++) {
        for (uint32_t j = 0; j < y; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

uint8_t *generate_zero_vector(uint32_t size) {
    uint8_t *vector = malloc(size);
    for (uint32_t i = 0; i < size; i++) {
        vector[i] = 0;
    }
    return vector;
}

uint8_t** generate_zero_matrix(uint32_t x, uint32_t y) {
    uint8_t **matrix = malloc(x * sizeof(uint8_t *));
    uint8_t *temp = malloc(x * y);
    for (int i = 0; i < x; i++) {
        matrix[i] = temp + (i * y);
        for (int j = 0; j < y; j++) matrix[i][j] = 0;
    }
    return matrix;
}


uint8_t **random_matrix(uint32_t n, uint32_t m) {
    uint8_t **matrix = malloc(n * sizeof(uint8_t *));
    uint8_t *temp = malloc(n * m);
    for (uint32_t i = 0; i < n; i++) {
        matrix[i] = temp + (i * m);
        for (uint32_t j = 0; j < m; j++) {
            matrix[i][j] = (uint8_t) (rand() % 256);
        }
    }
    return matrix;
}

void fill_matrix_random(uint8_t **matrix, uint32_t n, uint32_t m) {
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < m; j++) {
            matrix[i][j] = (uint8_t) (rand() % 256);
        }
    }
}