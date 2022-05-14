#include "./headers/test_tinymt32.h"
#include "./headers/test_system.h"
#include "../headers/debug.h"
#include "./headers/test_block.h"
#include "./headers/test_program.h"
#include <CUnit/Basic.h>


void generate_tinymt32_suite(CU_pSuite suite) {
    CU_add_test(suite, "correct_coeffs", test_tinymt32_gen_42);
}


void generate_system_suite(CU_pSuite suite) {
    CU_add_test(suite, "Correct vector addition", test_gf_256_full_add_vector);
    CU_add_test(suite, "Correct vector mult", test_gf_256_mul_vector);
    CU_add_test(suite, "Correct vector division", test_gf_256_inv_vector);
    CU_add_test(suite, "Correct gaussian elimination", test_gf_256_gaussian_elimination);
    CU_add_test(suite, "Correct coeffs gen", test_gen_coefs);
}



void generate_block_suite(CU_pSuite suite){
    CU_add_test(suite, "get_lost_words", test_find_lost_words);
    CU_add_test(suite, "get_info_from_buffer", test_get_file_info_from_buffer);
    
}

void generate_program_suite(CU_pSuite suite) {
    CU_add_test(suite, "test_program", test_program);
}


int main(int argc, char *argv[])
{
    if((argc == 2) && (strncmp(argv[1], "-v", 2) == 0)){
        
        ACTIVATE_DEBUG();
    }
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite tinymt32_suite = CU_add_suite("tinymt32", 0, 0);
    generate_tinymt32_suite(tinymt32_suite);

    CU_pSuite system_suite = CU_add_suite("system", system_setup, system_teardown);
    generate_system_suite(system_suite);

    CU_pSuite block_suite = CU_add_suite("block", system_setup, system_teardown);
    generate_block_suite(block_suite);

    CU_pSuite program_suite = CU_add_suite("program", system_setup, system_teardown);
    generate_program_suite(program_suite);

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
}