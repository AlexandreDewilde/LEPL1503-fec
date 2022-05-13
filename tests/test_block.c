#include "headers/test_block.h"
#include <CUnit/Basic.h>
#include <stdbool.h>



void test_get_file_info(){
    FILE *file;
    file_info_t *info= malloc(sizeof(file_info_t));
    
    file = fopen("samples/africa.bin","rb");
    get_file_info(file,info);

    CU_ASSERT_EQUAL(info->seed,1);
    CU_ASSERT_EQUAL(info->block_size,50);
    CU_ASSERT_EQUAL(info->word_size,100);
    CU_ASSERT_EQUAL(info->redudancy,20);
    CU_ASSERT_EQUAL(info->message_size,22832);


    file  = fopen("samples/big.bin","rb");
    get_file_info(file,info);
    CU_ASSERT_EQUAL(info->seed,1);
    CU_ASSERT_EQUAL(info->block_size,50);
    CU_ASSERT_EQUAL(info->word_size,1000);
    CU_ASSERT_EQUAL(info->redudancy,20);
    CU_ASSERT_EQUAL(info->message_size,52955);

    file  = fopen("samples/medium.bin","rb");
    get_file_info(file,info);
    CU_ASSERT_EQUAL(info->seed,12345);
    CU_ASSERT_EQUAL(info->block_size,10);
    CU_ASSERT_EQUAL(info->word_size,20);
    CU_ASSERT_EQUAL(info->redudancy,2);
    CU_ASSERT_EQUAL(info->message_size,783);

    file  = fopen("samples/small.bin","rb");
    get_file_info(file,info);
    CU_ASSERT_EQUAL(info->seed,42);
    CU_ASSERT_EQUAL(info->block_size,3);
    CU_ASSERT_EQUAL(info->word_size,3);
    CU_ASSERT_EQUAL(info->redudancy,4);
    CU_ASSERT_EQUAL(info->message_size,23);

    free(info);
}



void test_find_lost_words(){
    FILE *file;
    file = fopen("samples/small.bin","rb");

    block_t *block1 = malloc(sizeof(block_t));
    block1->block_size = 3;
    block1->word_size = 3;
    block1->message = malloc(block1->block_size * block1->word_size);
    for (uint32_t i = 0; i < block1->word_size* block1->block_size; i++) block1->message[i] = 0; 

    bool unknown_indexes[block1->block_size];
    
    CU_ASSERT_EQUAL(find_lost_words(block1, unknown_indexes), 3);

    free(block1->message);
    fclose(file);
    free(block1);
}




