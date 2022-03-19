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


void parse_matrix_file(const char *filename, uint32_t *NB, uint32_t **n, uint32_t **b_size, uint8_t ****A_matrices, uint8_t ****b_matrices, uint8_t ****solutions) {

    char *line = NULL;
    size_t len = 0;
    char *endPtr;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        return;
    }

    getline(&line, &len, fp);
    *NB = strtoul(line, NULL, 10);

    *A_matrices = malloc(*NB*sizeof(uint8_t***));
    *b_matrices = malloc(*NB* sizeof(uint8_t***));
    *solutions = malloc(*NB*sizeof(uint8_t***));
    *n = malloc(*NB * sizeof(uint32_t*));
    *b_size = malloc(*NB * sizeof(uint32_t*));

    for (uint32_t k = 0; k < *NB; k++) {
        getline(&line, &len, fp);
        getline(&line, &len, fp);
        (*n)[k] = strtoul(line, &endPtr, 10);
        (*b_size)[k] = strtoul(endPtr + 1, &endPtr, 10);
    
        (*A_matrices)[k] = generate_zero_matrix((*n)[k], (*n)[k]);
        (*b_matrices)[k] = generate_zero_matrix((*n)[k], (*b_size)[k]);
        (*solutions)[k] = generate_zero_matrix((*n)[k], (*b_size)[k]);

        for (uint32_t i = 0; i < (*n)[k]; i++) {
            getline(&line, &len, fp);
            endPtr = line;
            for (uint32_t j = 0; j < (*n)[k]; j++) {
                (*A_matrices)[k][i][j] = strtoul(endPtr, &endPtr, 10);
                endPtr++;
            }
        }
        for (uint32_t i = 0; i < (*n)[k]; i++) {
            getline(&line, &len, fp);
            endPtr = line;
            for (uint32_t j = 0; j < (*b_size)[k]; j++) {
                (*b_matrices)[k][i][j] = strtoul(endPtr, &endPtr, 10);
            }
        }
        for (uint32_t i = 0; i < (*n)[k]; i++) {
            getline(&line, &len, fp);
            endPtr = line;
            for (uint32_t j = 0; j < (*b_size)[k]; j++) {
                (*solutions)[k][i][j] = strtoul(endPtr, &endPtr, 10);
            }
        }
    }
    // fclose(fp);

}