#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int compare_2Darray(uint8_t** arr1, uint8_t** arr2, uint32_t size_x, uint32_t size_y);

uint8_t* generate_random_vector(uint32_t size);

void fill_vector_random(uint8_t *vector, uint32_t size);

uint8_t **deep_copy(uint8_t **arr, uint32_t x, uint32_t y);

void fill_matrix(uint8_t **src, uint8_t **dest, uint32_t x, uint32_t y);

uint8_t* generate_zero_vector(uint32_t size);

uint8_t** generate_zero_matrix(uint32_t x, uint32_t y);

uint8_t **random_matrix(uint32_t n, uint32_t m);

void fill_matrix_random(uint8_t **matrix, uint32_t n, uint32_t m);

void parse_matrix_file(const char *filename, uint32_t *NB, uint32_t **n, uint32_t **b_size, uint8_t ****A, uint8_t ****b, uint8_t ****solutions);