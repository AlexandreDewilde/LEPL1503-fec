#include "headers/generate_samples.h"


uint8_t** generate_zero_matrix(int x, int y) {
    uint8_t **matrix = malloc(x*sizeof(uint8_t*));
    for (int i = 0; i < x; i++) {
        matrix[i] = malloc(y);
        for (int j = 0; j < y; j++) matrix[i][j] = 0;
    }
    return matrix;
}