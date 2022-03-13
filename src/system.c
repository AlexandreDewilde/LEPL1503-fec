#include "../headers/system.h"

uint8_t *gf_256_full_add_vector(uint8_t *symbol_1, uint8_t *symbol_2, uint32_t symbol_size) {
    uint8_t *add_vector = (uint8_t *) malloc(symbol_size);
    if (add_vector == NULL) {
        return NULL;
    }
    for (uint32_t i = 0; i < symbol_size; i++) {
        add_vector[i] = symbol_1[i] ^ symbol_2[i];
    }
    return add_vector;
}


uint8_t *gf_256_mul_vector(uint8_t *symbol, uint8_t coef, uint32_t symbol_size) {
    uint8_t *mul_vector = (uint8_t *) malloc(symbol_size);
    if (mul_vector == NULL) {
        return NULL;
    }
    for (uint32_t i = 0; i < symbol_size; i++) {
        mul_vector[i] = gf256_mul_table[coef][symbol[i]];
    }
    return mul_vector;
}


uint8_t *gf_256_inv_vector(uint8_t *symbol, uint8_t coef, uint32_t symbol_size) {
    uint8_t *inv_vector = (uint8_t *) malloc(symbol_size);
    if (inv_vector == NULL) {
        return NULL;
    }
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
            if (sub_line == NULL) {
                exit(EXIT_FAILURE);
            }

            uint8_t *b_sub_line = gf_256_mul_vector(b[k], factor, symbol_size);

            if (b_sub_line == NULL) {
                // TODO 
                // how we deal with error?
            }

            A[i] = gf_256_full_add_vector(A[i], sub_line, system_size);

            if (A[i] == NULL) {
                // TODO 
            }

            b[i] = gf_256_full_add_vector(b[i], b_sub_line, symbol_size);

            if (b[i] == NULL) {
                // TODO 
            }

            free(sub_line);
            free(b_sub_line);
        }
    }

    // Subsituation  arrière
    for (uint32_t k = 1; k < system_size; k++) {
        uint32_t i = system_size - k;
        uint32_t j = i - 1;
        do {
            uint8_t factor = gf256_mul_table[A[j][i]][gf256_inv_table[A[i][i]]];
            uint8_t *b_sub_line = gf_256_mul_vector(b[i], factor, symbol_size);

            if (b_sub_line == NULL) {
                // TODO 
            }

            b[j] = gf_256_full_add_vector(b[j], b_sub_line, symbol_size);
            free(b_sub_line);

            if (b[j] == NULL) {
                // TODO 
            }
        } while (j--);

        b[i] = gf_256_inv_vector(b[i], A[i][i], symbol_size);
        if (b[i] == NULL) {
            // TODO 
        }
    }
}


uint8_t **gen_coefs(uint32_t seed, uint32_t nss, uint32_t nrs) {
    tinymt32_t prng;
    memset(&prng, 0, sizeof(tinymt32_t));
    prng.mat1 = 0x8f7011ee;
    prng.mat2 = 0xfc78ff1f;
    prng.tmat = 0x3793fdff;
    tinymt32_init(&prng, seed);

    uint8_t **coefs = malloc(nrs);
    for (uint32_t i = 0; i < nrs; i++) {
        coefs[i] = malloc(nss);
        if (coefs[i] == NULL) {
            return NULL;
        }
        for (uint32_t j = 0; j < nss; j++) {
            coefs[i][j] = (uint8_t) tinymt32_generate_uint32(&prng);
        }
    }
    return coefs;
}