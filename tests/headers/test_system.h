#ifndef TEST_SYSTEM
#define TEST_SYSTEM
#include "../../headers/system.h"
#include "tools.h"
#include <CUnit/Basic.h>


int system_setup();


int system_teardown();

/*
    a function that test our gf_256_full_add() function
*/
void test_gf_256_full_add_vector();

/*
    a function that test our gf_256_mul_vector() function
*/
void test_gf_256_mul_vector();


/*
    a function that test our gf_256_inv_vector() function
*/
void test_gf_256_inv_vector();

/*
    a function that test our gf_256_gaussian_elimination() function
*/

void test_gf_256_gaussian_elimination();


/*
   a fonction that test ou gen_coef() function
*/
void test_gen_coefs();

#endif /* TEST_SYSTEM */