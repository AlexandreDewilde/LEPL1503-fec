#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t **deep_copy(uint8_t **arr, uint32_t x, uint32_t y);

void fill_matrix(uint8_t **src, uint8_t **dest, uint32_t x, uint32_t y);

uint8_t** generate_zero_matrix(int x, int y);

uint8_t **random_matrix(uint32_t n, uint32_t m);

void fill_matrix_random(uint8_t **matrix, uint32_t n, uint32_t m);