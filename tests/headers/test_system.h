#ifndef TEST_SYSTEM
#define TEST_SYSTEM
#include "../../headers/system.h"
#include "generate_samples.h"
#include <CUnit/Basic.h>


int system_setup();


int system_teardown();


void test_gf_256_full_add_vector();


void test_gf_256_mul_vector();


void test_gf_256_inv_vector();

void test_gf_256_gaussian_elimination();


void test_gen_coefs();

#endif /* TEST_SYSTEM */