#include "headers/test_block.h"
#include <CUnit/Basic.h>
#include <stdbool.h>



void test_get_file_info(){
    FILE *file;
    file_info_t *info= malloc(sizeof(file_info_t));
    
    file = fopen("Binary_doc_test/africa.bin","rb");
    get_file_info(file,info);

    CU_ASSERT_EQUAL(info->seed,1);
    CU_ASSERT_EQUAL(info->block_size,50);
    CU_ASSERT_EQUAL(info->word_size,100);
    CU_ASSERT_EQUAL(info->redudancy,20);
    CU_ASSERT_EQUAL(info->message_size,22832);

    fclose(file);


    file  = fopen("Binary_doc_test/big.bin","rb");
    get_file_info(file,info);
    CU_ASSERT_EQUAL(info->seed,1);
    CU_ASSERT_EQUAL(info->block_size,50);
    CU_ASSERT_EQUAL(info->word_size,1000);
    CU_ASSERT_EQUAL(info->redudancy,20);
    CU_ASSERT_EQUAL(info->message_size,52955);

    fclose(file);

    file  = fopen("Binary_doc_test/medium.bin","rb");
    get_file_info(file,info);
    CU_ASSERT_EQUAL(info->seed,12345);
    CU_ASSERT_EQUAL(info->block_size,10);
    CU_ASSERT_EQUAL(info->word_size,20);
    CU_ASSERT_EQUAL(info->redudancy,2);
    CU_ASSERT_EQUAL(info->message_size,783);

    fclose(file);

    file  = fopen("Binary_doc_test/small.bin","rb");
    get_file_info(file,info);
    CU_ASSERT_EQUAL(info->seed,42);
    CU_ASSERT_EQUAL(info->block_size,3);
    CU_ASSERT_EQUAL(info->word_size,3);
    CU_ASSERT_EQUAL(info->redudancy,4);
    CU_ASSERT_EQUAL(info->message_size,23);

    fclose(file);

    free(info);
      
}



void test_find_lost_words(){
    FILE *file;
    file = fopen("Binary_doc_test/small.bin","rb");

    block_t *block1 = malloc(sizeof(block_t));
    block1->block_size = 3;
    block1->word_size = 3;
    block1->message = generate_zero_matrix(block1->block_size, block1->word_size);

    bool unknown_indexes[block1->block_size];
    
    CU_ASSERT_EQUAL(find_lost_words(block1, unknown_indexes), 3);

    free(block1->message[0]);
    free(block1->message);
    fclose(file);
    free(block1);
}




void test_prepare_block();
void test_make_block();
void test_free_blocks();
void test_make_linear_system();
void test_process_block();
void test_write_block();
void write_last_block();
void test_blocks_to_message_t();
void test_parse_file();


