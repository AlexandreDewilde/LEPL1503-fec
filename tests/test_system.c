#include "../headers/system.h"
#include <CUnit/Basic.h>


int setup() {
    return 0;
}


int teardown() {
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


int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("system", setup, teardown);
    CU_add_test(suite, "Correct vector addition", test_gf_256_full_add_vector);
    CU_add_test(suite, "Correct vector mult", test_gf_256_mul_vector);
    CU_add_test(suite, "Correct vector division", test_gf_256_inv_vector);
    CU_add_test(suite, "Correct gaussian elimination", test_gf_256_gaussian_elimination);
    CU_add_test(suite, "Correct coeffs gen", test_gen_coefs);

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
}