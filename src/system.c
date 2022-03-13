#include "../headers/system.h"

uint8_t *gf_256_full_add_vector(uint8_t *symbol_1, uint8_t *symbol_2, uint32_t symbol_size) {
    uint8_t *add_vector = (uint8_t *) malloc(symbol_size);
    for (uint32_t i = 0; i < symbol_size; i++) {
        add_vector[i] = symbol_1[i] ^ symbol_2[i];
    }
    return add_vector;
}


uint8_t *gf_256_mul_vector(uint8_t *symbol, uint8_t coef, uint32_t symbol_size) {
    uint8_t *mul_vector = (uint8_t *) malloc(symbol_size);
    for (uint32_t i = 0; i < symbol_size; i++) {
        mul_vector[i] = gf256_mul_table[coef][symbol[i]];
    }
    return mul_vector;
}


uint8_t *gf_256_inv_vector(uint8_t *symbol, uint8_t coef, uint32_t symbol_size) {
    uint8_t *inv_vector = (uint8_t *) malloc(symbol_size);
    
    for (uint32_t i = 0; i < symbol_size; i++) {
        inv_vector[i] = gf256_mul_table[symbol[i]][gf256_inv_table[coef]];
    }
    return inv_vector;
}


void gf_256_gaussian_elimination(uint8_t **A, uint8_t **b, uint32_t symbol_size, uint32_t system_size) {
    // Gauss elimination
    for (uint32_t k = 0; k < system_size; k++) {
        for (uint32_t i = k + 1; i < system_size; i++) {
            uint8_t factor = gf256_mul_table[A[i][k]][gf256_inv_table[A[k][k]]];
            uint8_t *sub_line = gf_256_mul_vector(A[k], factor, system_size);
            uint8_t *b_sub_line = gf_256_mul_vector(b[k], factor, symbol_size);
            A[i] = gf_256_full_add_vector(A[i], sub_line, system_size);
            b[i] = gf_256_full_add_vector(b[i], b_sub_line, symbol_size);
            free(sub_line);
            free(b_sub_line);
        }
    }

    // Subsituation  arrière
    for (uint32_t k = 1; k < system_size; k++)
    {
        uint32_t i = system_size - k;
        uint32_t j = i - 1;
        do
        {
            uint8_t factor = gf256_mul_table[A[j][i]][gf256_inv_table[A[i][i]]];
            uint8_t *b_sub_line = gf_256_mul_vector(b[i], factor, symbol_size);
            b[j] = gf_256_full_add_vector(b[j], b_sub_line, symbol_size);
            free(b_sub_line);
        } while (j--);

        b[i] = gf_256_inv_vector(b[i], A[i][i], symbol_size);
    }
}