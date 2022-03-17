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
    
    for (int i = 0; i < 3; i++) {
        free(A[i]);
        free(B[i]);
        free(ans[i]);
    }
    free(A);
    free(B);
    free(ans);
}


void test_gf_256_gaussian_elimination_basic() {
    // Basic test
    uint8_t A[3][3] = { {1,2,3}, {5,5,6}, {11,8,9} };
    uint8_t b[3][3] = { {0,1,2}, {1,1,1}, {2,2,2} };
    uint8_t ans[3][3] = { {49, 142, 82}, {150, 142, 166}, {0, 122, 244} };
    // Convert to double pointer **
    uint8_t *A_double_pointers[3] = {A[0], A[1], A[2]};
    uint8_t *b_double_pointers[3] = {b[0], b[1], b[2]};
    uint8_t *ans_double_pointers[3] = {ans[0], ans[1], ans[2]};

    gf_256_gaussian_elimination(A_double_pointers, b_double_pointers, 3, 3);
    CU_ASSERT_EQUAL(0, compare_2Darray(b_double_pointers, ans_double_pointers, 3, 3));
}

void test_gf_256_gaussian_elimination() {
    test_gf_256_gaussian_elimination_zero();
    test_gf_256_gaussian_elimination_basic();
}


void test_gen_coefs() {

}