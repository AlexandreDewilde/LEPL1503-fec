#include "../headers/program.h"
#define buffer_size 10




int in = 0;
int out = 0;
sem_t *full;
sem_t *empty;
pthread_mutex_t mutex;
file_thread_t buffer[buffer_size];


int in_writer = 0;
int out_writer = 0;
sem_t *full_writer;
sem_t *empty_writer;
pthread_mutex_t mutex_writer;
output_infos_t buffer_writer[buffer_size];

int skipped_buffer = 0;

args_t args;

void sem_error(char *function_name) {
    fprintf(stderr, "Error with semaphore functions %s :  %s\n", function_name, strerror(errno));
    exit(EXIT_FAILURE);
}

void mutex_error() {
    fprintf(stderr, "Error with mutex (un)lock / init / destroy function, %s", strerror(errno));
    exit(EXIT_FAILURE);
}


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

        file_thread_t current_file_thread = {
            .filename = malloc(strlen(directory_entry->d_name)+1), // Error treated after init
            .file = input_file
        };
        
        if (!current_file_thread.filename) {
            fprintf(stderr, "Error to allocate memory for filename\n");
            exit(EXIT_FAILURE);
        }
        strcpy(current_file_thread.filename, directory_entry->d_name);


        if (sem_wait(empty) != 0) sem_error("sem_wait");
        if(pthread_mutex_lock(&mutex) != 0) mutex_error();
        buffer[in] = current_file_thread;
        in = (in + 1) % buffer_size;
        if (pthread_mutex_unlock(&mutex) != 0) mutex_error();
        if (sem_post(full) != 0) sem_error("sem_post");
        
    }

    file_thread_t current_file_thread;
    memset(&current_file_thread, 0, sizeof(file_thread_t));
    
    for (int i = 0; i < args.nb_threads; i++) {

        if (sem_wait(empty) != 0) sem_error("sem_wait");
        if (pthread_mutex_lock(&mutex) != 0) mutex_error();;
        
        buffer[in] = current_file_thread;
        in = (in + 1) % buffer_size;
        if (pthread_mutex_unlock(&mutex) != 0) mutex_error();
        if (sem_post(full) != 0) sem_error("sem_post"); 
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
       
        if (sem_wait(full) != 0) sem_error("sem_wait");
        if (pthread_mutex_lock(&mutex) != 0) mutex_error();
        file_thread_t current_file_thread = buffer[out];
        out = (out + 1) % buffer_size;
        if (pthread_mutex_unlock(&mutex) != 0) mutex_error();
        if (sem_post(empty) != 0) sem_error("sem_post");

        output_infos_t current_output_info;

        parse_file(&current_output_info, &current_file_thread);
        

        if (sem_wait(empty_writer) != 0) sem_error("sem_wait");
        if (pthread_mutex_lock(&mutex_writer) != 0) mutex_error();
        buffer_writer[in_writer] = current_output_info;
        in_writer = (in_writer + 1) % buffer_size;
        if (pthread_mutex_unlock(&mutex_writer) != 0) mutex_error();
        if (sem_post(full_writer) != 0) sem_error("sem_post");
        
        if (!current_file_thread.filename) {
            break;
        }
        // Close this instance file
        fclose(current_file_thread.file);
    }
}

void consumer() {
    while (true) {

        if (sem_wait(full_writer) != 0) sem_error("sem_wait");
        if (pthread_mutex_lock(&mutex_writer) != 0) mutex_error();
        output_infos_t current_file_info = buffer_writer[out_writer];
        out_writer = (out_writer + 1) % buffer_size;
        if (pthread_mutex_unlock(&mutex_writer) != 0) mutex_error();
        if (sem_post(empty_writer) != 0) sem_error("sem_post");

        if (!current_file_info.filename) {
            skipped_buffer++;
            if (skipped_buffer >= args.nb_threads) break;
            continue;
        }
        

        write_to_file(&current_file_info, args.output_stream);
    }
}

int program(int argc, char *argv[]) {
    struct timeval *start = (struct timeval *) malloc(sizeof(struct timeval));
    struct timeval *end = (struct timeval *) malloc(sizeof(struct timeval));
    DEBUG_start_time(start);
    
    int err = parse_args(&args, argc, argv);
    if (err == -1)
    {
        exit(EXIT_FAILURE);
    }
    else if (err == 1)
    {
        exit(EXIT_SUCCESS);
    }

    DEBUG("\tnumber of threads executing the RLC decoding algorithm in parallel: %" PRIu32 "\n", args.nb_threads);
    DEBUG("\tverbose mode: %s\n", args.verbose ? "enabled" : "disabled");


    if (pthread_mutex_init(&mutex, NULL) != 0) mutex_error();
    if (pthread_mutex_init(&mutex_writer, NULL) != 0) mutex_error();
    empty = sem_init(buffer_size);
    empty_writer = sem_init(buffer_size);
    full = sem_init(0);
    full_writer = sem_init(0);

    pthread_t folder_thread;
    pthread_t prod[args.nb_threads];
    pthread_t cons;

    int error;
    if ((error = pthread_create(&folder_thread, NULL, (void *) folder_producer, NULL)) != 0) {
        fprintf(stderr, "Failed to create folder producer thread, error code : %d\n", error);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < args.nb_threads;i++) {
        if((error = pthread_create(&prod[i], NULL, (void *) producer, NULL)) != 0) {
            fprintf(stderr, "Failed to create producer thread number : %d, error code : %d", i, err);
            exit(EXIT_FAILURE);
        }
    }
    if ((err = pthread_create(&cons, NULL, (void *) consumer, NULL)) != 0) {
        fprintf(stderr, "Failed to create consumer thread, error code : %d\n", err);
        exit(EXIT_FAILURE);
    };


    if ((err = pthread_join(folder_thread, NULL)) != 0) {
        fprintf(stderr, "Failed to join folder producer thread, error code %d\n", err);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < args.nb_threads; i++) {
        if ((err = pthread_join(prod[i], NULL)) != 0) {
            fprintf(stderr, "Failed to join thread producer, number : %d, error code : %d", i, err);
            exit(EXIT_FAILURE);
        }
    }

    if ((err = pthread_join(cons, NULL)) != 0) {
        fprintf(stderr, "Failed to join consumer thread, error code : %d\n", err);
        exit(EXIT_FAILURE);
    }

    sem_destroy(empty_writer);
    sem_destroy(empty);
    sem_destroy(full);
    sem_destroy(full_writer);
    if (pthread_mutex_destroy(&mutex) != 0) mutex_error();
    if (pthread_mutex_destroy(&mutex_writer)) mutex_error();

    

    if (args.output_stream != stdout)
    {
        DEBUG_end_time(end);
        double used_time = DEBUG_get_delta_time(start,end);
        DEBUG_PRINT_TIME_USED(used_time);

        fclose(args.output_stream);
    }
    DEBUG_end_time(end);
    double used_time = DEBUG_get_delta_time(start,end);
    DEBUG_PRINT_TIME_USED(used_time);
    free(start);
    free(end);
    return 0;

    
}