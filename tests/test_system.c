#include "./headers/test_system.h"


int system_setup() {
    return 0;
}


int system_teardown() {
    return 0;
}


void test_gf_256_full_add_vector() {

    // Simple basic test case
    uint8_t add_vector[] = {1,2,3,4};
    uint8_t add_vector2[] = {4,3,2,1};
    uint8_t ans[] = {5,1,1,5};
    uint8_t *res = gf_256_full_add_vector(add_vector, add_vector2, 4);

    CU_ASSERT_EQUAL(0, memcmp(res, ans, 4));
}


void test_gf_256_mul_vector() {

}


void test_gf_256_inv_vector() {

}


void test_gf_256_gaussian_elimination() {

}


void test_gen_coefs() {

}