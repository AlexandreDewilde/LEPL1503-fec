#include "./headers/test_tinymt32.h"
#include "./headers/test_system.h"
#include <CUnit/Basic.h>


/**
*
*
* @param
* @return: 
*/

void generate_tinymt32_suite(CU_pSuite suite) {
    CU_add_test(suite, "correct_coeffs", test_tinymt32_gen_42);
}

/**
*
*
* @param
* @return: 
*/

void generate_system_suite(CU_pSuite suite) {
    CU_add_test(suite, "Correct vector addition", test_gf_256_full_add_vector);
    CU_add_test(suite, "Correct vector mult", test_gf_256_mul_vector);
    CU_add_test(suite, "Correct vector division", test_gf_256_inv_vector);
    CU_add_test(suite, "Correct gaussian elimination", test_gf_256_gaussian_elimination);
    CU_add_test(suite, "Correct coeffs gen", test_gen_coefs);
}

int main()
{
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite tinymt32_suite = CU_add_suite("tinymt32", 0, 0);
    generate_tinymt32_suite(tinymt32_suite);

    CU_pSuite system_suite = CU_add_suite("system", system_setup, system_teardown);
    generate_system_suite(system_suite);
    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
}