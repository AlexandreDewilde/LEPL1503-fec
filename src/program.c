#include "../headers/program.h"
#define buffer_size 8

int in = 0;
int in_writer = 0;
int out = 0;
int out_writer = 0;
bool folder_readed = false;
uint64_t nb_files = 0;
uint64_t file_parsed = 0;
uint64_t file_written = 0;

sem_t *full;
sem_t *full_writer;
sem_t *empty;
sem_t *empty_writer;
file_thread buffer[buffer_size];
output_infos_t buffer_writer[buffer_size];
pthread_mutex_t mutex;
pthread_mutex_t mutex_writer;
pthread_mutex_t mutex_variables;

args_t args;



void folder_producer() {
    struct dirent *directory_entry;
    FILE *input_file;
    while ((directory_entry = readdir(args.input_dir)))
    {
        // Ignore parent and current directory
        if (!strcmp(directory_entry->d_name, "."))
        {
            continue;
        }
        if (!strcmp(directory_entry->d_name, ".."))
        {
            continue;
        }

        // Add the directory path to the filename to open it
        char full_path[PATH_MAX];
        memset(full_path, 0, sizeof(char) * PATH_MAX);
        strcpy(full_path, args.input_dir_path);
        strcat(full_path, "/");
        strcat(full_path, directory_entry->d_name);

        input_file = fopen(full_path, "rb");
        if (input_file == NULL)
        {
            fprintf(stderr, "Failed to open the input file %s: %s\n", full_path, strerror(errno));
            goto file_read_error;
        }

        // This is a simple example of how to use the verbose mode
        DEBUG("Successfully opened the file %s\n", full_path);

        file_thread current_file_thread;
        current_file_thread.filename = malloc(strlen(directory_entry->d_name)+1);
        strcpy(current_file_thread.filename, directory_entry->d_name);
        current_file_thread.file = input_file;
        current_file_thread.output = args.output_stream;

        sem_wait(empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = current_file_thread;
        in = (in + 1) % buffer_size;
        // pthread_mutex_lock(&mutex_variables);
        nb_files++;
        // pthread_mutex_unlock(&mutex_variables);
        pthread_mutex_unlock(&mutex);
        sem_post(full);
        
    }
    pthread_mutex_lock(&mutex_variables);
    folder_readed = true;
    pthread_mutex_unlock(&mutex_variables);

    for (int i = 0; i < args.nb_threads; i++) {

        sem_wait(empty);
        pthread_mutex_lock(&mutex);
        file_thread current_file_thread;
        current_file_thread.filename = NULL;
        buffer[in] = current_file_thread;
        in = (in + 1) % buffer_size;
        pthread_mutex_unlock(&mutex);
        sem_post(full); 
    }

    
    // Close the input directory and the output file
    int err = closedir(args.input_dir);
    if (err < 0)
    {
        fprintf(stderr, "Error while closing the input directory containing the instance files\n");
    }
    return;

file_read_error:
    err = closedir(args.input_dir);
    if (err < 0)
    {
        fprintf(stderr, "Error while closing the input directory containing the instance files\n");
    }
    if (args.output_stream != stdout)
    {
        fclose(args.output_stream);
    }
    exit(EXIT_FAILURE);
}


void producer() {
    while (true) {
       
        sem_wait(full);
        pthread_mutex_lock(&mutex);
        file_thread current_file_thread = buffer[out];
        out = (out + 1) % buffer_size;
        pthread_mutex_unlock(&mutex);
        sem_post(empty);

        if (!current_file_thread.filename) break;

        output_infos_t current_output_info;
        file_info_t file_info;
        get_file_info(current_file_thread.file, &file_info);
        
        uint8_t **coeffs = gen_coefs(file_info.seed, file_info.block_size, file_info.word_size);
        verbose_matrix(coeffs, file_info.word_size, file_info.block_size);
        
        uint64_t step = file_info.word_size * (file_info.block_size + file_info.redudancy);
        uint64_t nb_blocks = ceil(file_info.file_size / (double) step);

        block_t *blocks = malloc(nb_blocks*sizeof(block_t));
        if (blocks == NULL) {
            DEBUG("Error allocating memory for block\n");
            exit(EXIT_FAILURE);
        }

        bool uncomplete_block = file_info.message_size != nb_blocks * file_info.block_size * file_info.word_size; 

        current_output_info.message_size = htobe64(file_info.message_size);

        for (uint64_t i = 0; i < nb_blocks - uncomplete_block; i++) {
            prepare_block(&blocks[i], file_info.block_size, file_info.word_size, file_info.redudancy);
            make_block(current_file_thread.file, &blocks[i]);
            process_block(&blocks[i], coeffs);
        }

        int64_t current_pos = ftell(current_file_thread.file);
        if (current_pos == -1) {
            DEBUG("Error with ftell : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        uint32_t remaining = ( (file_info.file_size + 24 - current_pos) / file_info.word_size) - file_info.redudancy;

        uint32_t padding = (file_info.block_size * file_info.word_size * (nb_blocks - 1)) + remaining * file_info.word_size - file_info.message_size;
        
        if (uncomplete_block) {
            prepare_block(&blocks[nb_blocks-1], remaining, file_info.word_size, file_info.redudancy);
            make_block(current_file_thread.file, &blocks[nb_blocks-1]);
            process_block(&blocks[nb_blocks-1], coeffs);
        }
        current_output_info.blocks = blocks;
        current_output_info.nb_blocks = nb_blocks;
        current_output_info.padding = padding;
        current_output_info.remaining = remaining;
        current_output_info.output = current_file_thread.output;
        current_output_info.uncomplete_block = uncomplete_block;
        current_output_info.filename = current_file_thread.filename;

        sem_wait(empty_writer);
        pthread_mutex_lock(&mutex_writer);
        buffer_writer[in_writer] = current_output_info;
        in_writer = (in_writer + 1) % buffer_size;
        pthread_mutex_unlock(&mutex_writer);
        sem_post(full_writer);
        free(coeffs[0]);
        free(coeffs);
        // Close this instance file
        fclose(current_file_thread.file);
    }
}

void consumer() {
    while (true) {
        pthread_mutex_lock(&mutex_variables);
        if (folder_readed && nb_files <= file_written) {
            pthread_mutex_unlock(&mutex_variables);
            break;
        }
        pthread_mutex_unlock(&mutex_variables);

        sem_wait(full_writer);
        pthread_mutex_lock(&mutex_writer);
        output_infos_t current_file_info = buffer_writer[out_writer];
        out_writer = (out_writer + 1) % buffer_size;
        file_written++;
        pthread_mutex_unlock(&mutex_writer);
        sem_post(empty_writer);
        

        uint32_t filename_length = htobe32(strlen(current_file_info.filename));
        size_t written = fwrite(&filename_length, sizeof(uint32_t), 1, current_file_info.output);
        if (written != 1) {
            DEBUG("Error writing to output the length of filename");
            exit(EXIT_FAILURE);
        }
        written = fwrite(&current_file_info.message_size, sizeof(uint64_t), 1, current_file_info.output);
        if (written != 1) {
            DEBUG("Error writing to output the message size\n");
            exit(EXIT_FAILURE);
        }
        written = fwrite(current_file_info.filename, strlen(current_file_info.filename), 1, current_file_info.output);
        if (written != 1) {
            printf("Error writing to output the filename\n");
            exit(EXIT_FAILURE);
        }
        for (uint64_t i = 0; i < current_file_info.nb_blocks - current_file_info.uncomplete_block; i++) {
            write_block(&current_file_info.blocks[i], current_file_info.output);
        }
        
        if (current_file_info.uncomplete_block) {
            write_last_block(&current_file_info.blocks[current_file_info.nb_blocks-1], current_file_info.output, current_file_info.remaining, current_file_info.padding);
        }
        
        free_blocks(current_file_info.blocks, current_file_info.nb_blocks);
        free(current_file_info.filename);
    }
}

int program(int argc, char *argv[]) {
    
    int err = parse_args(&args, argc, argv);
    if (err == -1)
    {
        exit(EXIT_FAILURE);
    }
    else if (err == 1)
    {
        exit(EXIT_SUCCESS);
    }
    // The following lines (and every code already present in this skeleton) can be removed, it is just an example to show you how to use the program arguments
    DEBUG("\tnumber of threads executing the RLC decoding algorithm in parallel: %" PRIu32 "\n", args.nb_threads);
    DEBUG("\tverbose mode: %s\n", args.verbose ? "enabled" : "disabled");

    // This is an example of how to open the instance files of the input directory. You may move/edit it during the project
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_writer, NULL);
    pthread_mutex_init(&mutex_variables, NULL);
    empty = sem_init(buffer_size);
    empty_writer = sem_init(buffer_size);
    full = sem_init(0);
    full_writer = sem_init(0);

    pthread_t folder_thread;
    pthread_t prod[args.nb_threads];
    pthread_t cons;
    pthread_create(&folder_thread, NULL, (void *) folder_producer, NULL);
    for (int i = 0; i < args.nb_threads;i++)
        pthread_create(&prod[i], NULL, (void *) producer, NULL);
    pthread_create(&cons, NULL, (void *) consumer, NULL);

    pthread_join(folder_thread, NULL);
    for (int i = 0; i < args.nb_threads; i++)
        pthread_join(prod[i], NULL);
    pthread_join(cons, NULL);
    sem_destroy(empty_writer);
    sem_destroy(empty);
    sem_destroy(full);
    sem_destroy(full_writer);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_writer);
    pthread_mutex_destroy(&mutex_variables);
    if (args.output_stream != stdout)
    {
        fclose(args.output_stream);
    }
    return 0;
}