#include "headers/test_block.h"
#include <CUnit/Basic.h>
#include <stdbool.h>


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


void test_get_file_info_from_buffer(){

    FILE *file;
    file_info_t *file_info = malloc(sizeof(file_info_t));
    file_info->file_size = 24;
    uint8_t *buffer = malloc(file_info->file_size);

    file = fopen("samples/africa.bin","rb");
    fread(buffer, 1, file_info->file_size, file);
    get_file_info_from_buffer(buffer,file_info);

    CU_ASSERT_EQUAL(file_info->seed,1);
    CU_ASSERT_EQUAL(file_info->block_size,50);
    CU_ASSERT_EQUAL(file_info->word_size,100);
    CU_ASSERT_EQUAL(file_info->redudancy,20);
    CU_ASSERT_EQUAL(file_info->message_size,22832);




    file = fopen("samples/africa.bin","rb");
    fread(buffer, 1, file_info->file_size, file);
    get_file_info_from_buffer(buffer,file_info);


    CU_ASSERT_EQUAL(file_info->seed,1);
    CU_ASSERT_EQUAL(file_info->block_size,50);
    CU_ASSERT_EQUAL(file_info->word_size,100);
    CU_ASSERT_EQUAL(file_info->redudancy,20);
    CU_ASSERT_EQUAL(file_info->message_size,22832);


    file  = fopen("samples/big.bin","rb");
    fread(buffer, 1, file_info->file_size, file);
    get_file_info_from_buffer(buffer,file_info);

    CU_ASSERT_EQUAL(file_info->seed,1);
    CU_ASSERT_EQUAL(file_info->block_size,50);
    CU_ASSERT_EQUAL(file_info->word_size,1000);
    CU_ASSERT_EQUAL(file_info->redudancy,20);
    CU_ASSERT_EQUAL(file_info->message_size,52955);

    file  = fopen("samples/medium.bin","rb");
    fread(buffer, 1, file_info->file_size, file);
    get_file_info_from_buffer(buffer,file_info);
   
    CU_ASSERT_EQUAL(file_info->seed,12345);
    CU_ASSERT_EQUAL(file_info->block_size,10);
    CU_ASSERT_EQUAL(file_info->word_size,20);
    CU_ASSERT_EQUAL(file_info->redudancy,2);
    CU_ASSERT_EQUAL(file_info->message_size,783);

    file  = fopen("samples/small.bin","rb");
  
    get_file_info_from_buffer(buffer,file_info);
    fread(buffer, 1, file_info->file_size, file);
    get_file_info_from_buffer(buffer,file_info);

    CU_ASSERT_EQUAL(file_info->seed,42);
    CU_ASSERT_EQUAL(file_info->block_size,3);
    CU_ASSERT_EQUAL(file_info->word_size,3);
    CU_ASSERT_EQUAL(file_info->redudancy,4);
    CU_ASSERT_EQUAL(file_info->message_size,23);


    fclose(file);
    free(file_info);
    free(buffer);

}






void test_prepare_block();
void test_free_blocks();
void test_make_linear_system();
void test_process_block();
void test_write_block();
void write_last_block();
void test_blocks_to_message_t();
void test_parse_file();


