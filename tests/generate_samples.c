#include "headers/generate_samples.h"


uint8_t** generate_zero_matrix(int x, int y) {
    uint8_t **matrix = (uint8_t **) malloc(x);
    for (int i = 0; i < x; i++) {
        matrix[i] = (uint8_t*) calloc(0, y);
    }
    return matrix;
}