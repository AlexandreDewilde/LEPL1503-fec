#include "headers/test_system.h"

int system_setup() {
    return 0;
}


int system_teardown() {
    return 0;
}

void test_gf_256_full_add_vector_simple_test() {
    // Simple basic test case
    uint8_t add_vector[] = {1,2,3,4};
    uint8_t add_vector2[] = {4,3,2,1};
    uint8_t ans[] = {5,1,1,5};
    uint8_t *res = gf_256_full_add_vector(add_vector, add_vector2, 4);

    CU_ASSERT_EQUAL(0, memcmp(res, ans, 4));
    
    free(res);
}

void test_gf_256_full_add_vector_random_test(uint32_t vector_size) {
    uint8_t *random_vector1 = generate_random_vector(vector_size);
    uint8_t *random_vector2 = generate_random_vector(vector_size);
    uint8_t *res = gf_256_full_add_vector(random_vector1, random_vector2, vector_size);

    for (uint32_t i = 0; i < vector_size; i++) {
        CU_ASSERT_EQUAL(random_vector1[i]^random_vector2[i], res[i]);
    }

    free(res);
    free(random_vector1);
    free(random_vector2);
}

void test_gf_256_full_add_vector() {
    test_gf_256_full_add_vector_simple_test();
    test_gf_256_full_add_vector_random_test(100);
}


void test_gf_256_mul_vector() {

}


void test_gf_256_inv_vector() {

}

void test_gf_256_gaussian_elimination_zero() {
    // Test with zero matrices
    uint8_t **A = generate_zero_matrix(3, 3);
    uint8_t **B = generate_zero_matrix(3, 1);
    uint8_t **ans = generate_zero_matrix(3, 1);

    gf_256_gaussian_elimination(A, B, 1, 3);
    CU_ASSERT_EQUAL(0, compare_2Darray(B, ans, 3, 1));
    
    free(A[0]);
    free(B[0]);
    free(ans[0]);
    free(A);
    free(B);
    free(ans);
}


void test_gf_256_gaussian_elimination_basic() {
    // Basic test
    uint8_t A[4][4] = { {42, 84, 8, 220}, {10, 222, 3, 92}, {173, 66, 95, 5 }, {83, 198, 39, 92 } };
    uint8_t b[4][6] = { {218, 199, 87, 225, 15, 18}, {6, 130, 239, 127, 79, 61}, {33, 59, 154, 23, 199, 158}, {250, 220, 212, 70, 1, 177 } };
    uint8_t ans[4][6] = { {165, 168, 238, 27, 54, 152 }, {24, 36, 137, 64, 207, 181 }, {64, 30, 221, 57, 40, 229}, {14, 240, 94, 138, 110, 177 } };
    // Convert to double pointer **
    uint8_t *A_double_pointers[4] = {A[0], A[1], A[2], A[3]};
    uint8_t *b_double_pointers[4] = {b[0], b[1], b[2], b[3]};
    uint8_t *ans_double_pointers[4] = {ans[0], ans[1], ans[2], ans[3]};

    gf_256_gaussian_elimination(A_double_pointers, b_double_pointers, 6, 4);
    CU_ASSERT_EQUAL(0, compare_2Darray(b_double_pointers, ans_double_pointers, 4, 6));
}

void test_gf_256_gaussian_elimination_file() {
    uint32_t NB, *n, *b_size;
    uint8_t ***A_matrices, ***B_matrices, ***solutions;
    parse_matrix_file("tests/samples/systems.txt", &NB, &n, &b_size, &A_matrices, &B_matrices, &solutions);

    for (uint32_t k = 0; k < NB; k++) {
        printf("%d, %d\n", n[k], b_size[k]);

        gf_256_gaussian_elimination(A_matrices[k], B_matrices[k], b_size[k], n[k]);
        CU_ASSERT_EQUAL(0, compare_2Darray(B_matrices[k], solutions[k], n[k], b_size[k]));
        for (uint32_t i = 0; i < n[k]; i++) {
            for (uint32_t j = 0; j < b_size[k]; j++) {
                printf("%d ", B_matrices[k][i][j]);
            }
            printf("\n");
        }
    }
}

void test_gf_256_gaussian_elimination() {
    test_gf_256_gaussian_elimination_zero();
    test_gf_256_gaussian_elimination_basic();
    test_gf_256_gaussian_elimination_file();
}


void test_gen_coefs() {
    uint8_t** res = gen_coefs(42, 2, 2);
    uint8_t ans[2][2] = {{171, 165}, {55, 61}};
    CU_ASSERT_EQUAL(0, memcmp(res[0], ans[0], 2));
    CU_ASSERT_EQUAL(0, memcmp(res[1], ans[1], 2));

    free(res[0]);
    free(res);
}