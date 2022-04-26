#include "headers/test_program.h"

char* read_file_output(FILE* file, char* filename) {
    uint32_t length_filename;
    uint64_t message_size;
    fread(&(length_filename), sizeof(uint32_t), 1, file);
    length_filename = be32toh(length_filename);
    fread(&message_size, sizeof(uint64_t), 1, file);
    message_size = be64toh(message_size);
    fread(filename, length_filename, 1, file);
    filename[length_filename] = '\0';
    char *message = malloc(message_size + 1);
    fread(message, message_size, 1, file);
    message[message_size] = '\0';
    return message;
}

void test_one_file() {
    char *argv[] = {"./fec", "tests/samples/sample_one_file", "-f", "test.txt"};
    thread_program(4, argv);
    
    FILE *file = fopen("test.txt", "rb");
    FILE *file2 = fopen("tests/samples/output_africa.bin", "rb");
    char *filename = malloc(25);
    char *filename2 = malloc(25);
    char* str = read_file_output(file, filename);
    char* str2 = read_file_output(file2, filename2);
    CU_ASSERT_EQUAL(0, strcmp(str, str2));
    free(str2);
    free(filename2);
    free(str);
    free(filename);
    fclose(file);
    fclose(file2);
}

void test_multiple_file() {
    char *argv[] = {"./fec", "Binary_doc_test", "-f", "test.txt"};
    thread_program(4, argv);
    char *output_folder = "tests/samples/";

    FILE *file = fopen("test.txt", "rb");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char *filename = malloc(25);
    while(ftell(file) < size) {
        char *str = read_file_output(file, filename);

        uint32_t length_path = strlen(output_folder) + 7 + strlen(filename) + 1;
        char * result_file = (char *) malloc(sizeof(char) * length_path);
        sprintf(result_file, "%soutput_%s", output_folder, filename);
        result_file[length_path-1] = '\0';
        FILE *file2 = fopen(result_file, "rb");

        char *filename2 = malloc(strlen(filename) + 1);
        char *str2 = read_file_output(file2, filename2);

        CU_ASSERT_EQUAL(0, strcmp(str, str2));
        
        free(result_file);
        free(str2);
        free(str);
        free(filename2);
        fclose(file2);
    }
    free(filename);
    fclose(file);
}

void test_program() {
    // test_one_file(); create another tests beceause if reset optind to 1 cause memory error say valgrind
    test_multiple_file();
}