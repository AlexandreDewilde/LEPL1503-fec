#ifndef TEST_SYSTEM
#define TEST_SYSTEM
#include "../../headers/system.h"
#include "tools.h"
#include <CUnit/Basic.h>


int system_setup();


int system_teardown();

/*
* This function test the gf_256_full_add_vector function of the program
*/
void test_gf_256_full_add_vector();

/*
* This function test the gf_256_mul_vector function of the program
*/
void test_gf_256_mul_vector();

/*
* This function test the gf_256_inv_vector function of the program
*/
void test_gf_256_inv_vector();

/*
* This function test the gf_256_gaussian_elimination function of the program
*/
void test_gf_256_gaussian_elimination();

/*
* This function test the gen_coefs function of the program
*/
void test_gen_coefs();

#endif /* TEST_SYSTEM */