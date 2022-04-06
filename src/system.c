#include "../headers/system.h"


/**
*
*
* @param
* @return: 
*/

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


/**
*
*
* @param
* @return: 
*/

void inplace_gf_256_full_add_vector(uint8_t *symbol_1, uint8_t *symbol_2, uint32_t symbol_size) {
    for (uint32_t i = 0; i < symbol_size; i++) {
        symbol_1[i] ^= symbol_2[i];
    }
}

/**
*
*
* @param
* @return: 
*/

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


/**
*
*
* @param
* @return: 
*/

void inplace_gf_256_mul_vector(uint8_t *symbol, uint8_t coef, uint32_t symbol_size) {
    for (uint32_t i = 0; i < symbol_size; i++) {
        symbol[i] = gf256_mul_table[coef][symbol[i]];
    }
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


/**
*
*
* @param
* @return: 
*/

void inplace_gf_256_inv_vector(uint8_t *symbol, uint8_t coef, uint32_t symbol_size) {
    for (uint32_t i = 0; i < symbol_size; i++) {
        symbol[i] = gf256_mul_table[symbol[i]][gf256_inv_table[coef]];
    }
}

/**
*
*
* @param
* @return: 
*/

void gf_256_gaussian_elimination_forward(uint8_t **A, uint8_t **b, uint32_t symbol_size, uint32_t system_size) {
    for (uint32_t k = 0; k < system_size; k++) {
        for (uint32_t i = k + 1; i < system_size; i++) {
            if (A[i][k] == 0) continue;
            uint8_t factor = gf256_mul_table[A[i][k]][gf256_inv_table[A[k][k]]];
            
            for (uint32_t j = 0; j < system_size; j++) {
                A[i][j] ^= gf256_mul_table[A[k][j]][factor];
            }

            for (uint32_t j = 0; j < symbol_size; j++) {
                b[i][j] ^= gf256_mul_table[b[k][j]][factor];
            }
            
            
        }
    }
}

/**
*
*
* @param
* @return: 
*/

void gf_256_gaussian_elimination_backward(uint8_t **A, uint8_t **b, uint32_t symbol_size, uint32_t system_size) {
    // Subsituation  arrière
    
    // for (uint32_t i = 0; i < system_size; i++) {
    //     for (uint32_t j = 0; j < system_size;j++) printf("%d ", A[i][j]);
    //     printf("\n");
    // }

    uint32_t i = system_size - 1;
    do {
        for (uint32_t j = i + 1; j < system_size; j++) {
            for (uint32_t k = 0; k < symbol_size; k++) {
                b[i][k] ^= gf256_mul_table[b[j][k]][A[i][j]];
            }
        }
        inplace_gf_256_inv_vector(b[i], A[i][i], symbol_size);

    } while(i--);
}

/**
*
*
* @param
* @return: 
*/

void gf_256_gaussian_elimination(uint8_t **A, uint8_t **b, uint32_t symbol_size, uint32_t system_size) {
    gf_256_gaussian_elimination_forward(A, b, symbol_size, system_size);
    gf_256_gaussian_elimination_backward(A, b, symbol_size, system_size);
}


uint8_t **gen_coefs(uint32_t seed, uint32_t nss, uint32_t nrs) {
    tinymt32_t prng;
    memset(&prng, 0, sizeof(tinymt32_t));
    prng.mat1 = 0x8f7011ee;
    prng.mat2 = 0xfc78ff1f;
    prng.tmat = 0x3793fdff;
    tinymt32_init(&prng, seed);

    // make use of cache memory for perfomance
    uint8_t **coefs = malloc(nrs * sizeof(uint8_t *));
    uint8_t *temp = malloc(nrs * nss);

    for (uint32_t i = 0; i < nrs; i++) {
        coefs[i] = temp + (i * nss);
    }

    for (uint32_t i = 0; i < nrs; i++) {
        for (uint32_t j = 0; j < nss; j++) {
            coefs[i][j] = (uint8_t) tinymt32_generate_uint32(&prng);
        }
    }

    return coefs;
}