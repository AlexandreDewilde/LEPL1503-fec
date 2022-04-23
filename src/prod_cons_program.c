#include "../headers/prod_cons_program.h"
#include "../headers/producer.h"
#include "../headers/shared_buffer.h"


typedef struct
{
    DIR *input_dir;
    char input_dir_path[PATH_MAX];
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;

typedef struct
{
    struct dirent *directory_entry;
    FILE *input_file;
    int nb_input_threads;
    int nb_of_file;
    args_t args;
    buffer_info **buffer;
    int prod_index;
    
}thread_need_info;

/**
*
*
* @param
* @return: 
*/

void usage(char *prog_name)
{
    fprintf(stderr, "USAGE:\n");
    fprintf(stderr, "    %s [OPTIONS] input_dir\n", prog_name);
    fprintf(stderr, "    input_dir: path to the directory containing the instance files with the encoded messages\n");
    fprintf(stderr, "    -f output_file: path to the output file containing all decoded messages\n");
    fprintf(stderr, "    -n n_threads (default: 4): set the number of computing threads that will be used to execute the RLC algorithm\n");
    fprintf(stderr, "    -v : enable debugging messages. If not set, no such messages will be displayed (except error messages on failure)\n");
}

void thread_consume(buffer_info **buffer, FILE *output){
    consume_in_shared_buffer(buffer, output);
    
}

thread_need_info *ignore_parent(void *argc){
    thread_need_info *argc_cmp = (thread_need_info *) argc;
    while ((argc_cmp->directory_entry = readdir(argc_cmp->args.input_dir)))
    {
        // Ignore parent and current directory
        if (!strcmp(argc_cmp->directory_entry->d_name, "."))
        {
            continue;
        }
        if (!strcmp(argc_cmp->directory_entry->d_name, ".."))
        {
            continue;
        }
    }

    thread_need_info *info = argc_cmp;
    return info;
}
void *thread_producer(void *args){
    thread_need_info *info = ignore_parent(args);

    // Add the directory path to the filename to open it
    char full_path[PATH_MAX];
    memset(full_path, 0, sizeof(char) * PATH_MAX);
    strcpy(full_path, info->args.input_dir_path);
    strcat(full_path, "/");
    strcat(full_path, info->directory_entry->d_name);

    info->input_file = fopen(full_path, "rb");
    if (info->input_file == NULL)
    {
        fprintf(stderr, "Failed to open the input file %s: %s\n", full_path, strerror(errno));
        goto file_read_error;
    }


    char *filename = info->directory_entry->d_name;
    producer(filename, info->input_file, info->buffer, info->nb_of_file); // here we call the function that creates blocks and deal with them
    // Close this instance file
    fclose(info->input_file);

    

    // Close the input directory and the output file
    int err = closedir(info->args.input_dir);
    if (err < 0)
    {
        fprintf(stderr, "Error while closing the input directory containing the instance files\n");
    }
    if (info->args.output_stream != stdout)
    {
        fclose(info->args.output_stream);
    }

    file_read_error:
    err = closedir(info->args.input_dir);
    if (err < 0)
    {
        fprintf(stderr, "Error while closing the input directory containing the instance files\n");
    }
    if (info->args.output_stream != stdout)
    {
        fclose(info->args.output_stream);
    }
    exit(EXIT_FAILURE);

    
    

}

void wait_free_threads(pthread_t *threads, int nb_input_threads){
    for (int i = 0 ; i < nb_input_threads; i++){
        int ret = pthread_join(threads[i], NULL);
        if(ret!=0) {
            perror("pthread_join error\n");
        }
    }
}

/**
 * The function opens the file and do the rest
 *
 * 
*/

void thread_parse_file(int nb_prod_threads, int nb_of_file, args_t args, buffer_info **buffer){
    // This is an example of how to open the instance files of the input directory. You may move/edit it during the project
    struct dirent *directory_entry;
    //FILE *input_file;
     // Here you have to create a thread for each loop entry but N threads must be the number of threads that are given
    pthread_t threads[nb_prod_threads];
    int thread_index = 0;

    thread_need_info *info = (thread_need_info *) malloc(sizeof(thread_need_info));
    info->nb_input_threads = nb_prod_threads;
    info->nb_of_file = nb_of_file;
    info->buffer = buffer;
    info->prod_index = prod_index;
    info->args = args;


    while ((directory_entry = readdir(args.input_dir)))
    {
        info->directory_entry = directory_entry;
        if (thread_index < nb_prod_threads){
            int err =pthread_create(&(threads[thread_index]),NULL, thread_producer, (void *)info);
            if(err!=0) {
                perror("pthread_create error\n");
            }
            thread_index += 1;
        }else{
            // wait for all threads to be free then restart till the end of folder
            wait_free_threads(threads, nb_prod_threads);
            thread_index = 0;

            int err =pthread_create(&(threads[thread_index]),NULL, thread_producer, (void *)info);
            if(err!=0) {
                perror("pthread_create error\n");
            }
            thread_index += 1;
        }
        
        
    }

    wait_free_threads(threads, nb_prod_threads);


    
}
/**
*
*
* @param
* @return: 
*/

int parse_args(args_t *args, int argc, char *argv[])
{
    memset(args, 0, sizeof(args_t));

    // Default values of the arguments
    args->nb_threads = 4;
    args->verbose = false;
    args->output_stream = stdout;
    int opt;
    while ((opt = getopt(argc, argv, "n:vf:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            args->nb_threads = atoi(optarg);
            if (args->nb_threads == 0)
            {
                fprintf(stderr, "The number of computing threads must be a positive integer, got: %s\n", optarg);
                return -1;
            }
            break;
        case 'v':
            args->verbose = true;
            ACTIVATE_DEBUG();
            break;
        case 'f':
            args->output_stream = fopen(optarg, "w");
            if (args->output_stream == NULL)
            {
                fprintf(stderr, "Impossible to open the output file %s: %s\n", optarg, strerror(errno));
                return -1;
            }
            break;
        case '?':
            usage(argv[0]);
            return 1;
        default:
            usage(argv[0]);
        }
    }

    if (optind == argc)
    {
        fprintf(stderr, "You must provide an input directory containing the instance files!\n");
        return -1;
    }

    // Source: https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
    if (NULL == (args->input_dir = opendir(argv[optind])))
    {
        fprintf(stderr, "Impossible to open the directory containing the input instance files %s: %s\n", argv[optind], strerror(errno));
        return -1;
    }
    // The following line is not very secure... Ask Prof. Legay and/or wait for LINGI2144 for more information :-)
    strcpy(args->input_dir_path, argv[optind++]);

    
    return 0;
}

int threads_program(int argc, char *argv[]) {
    args_t args;
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
    fprintf(stderr, "\tnumber of threads : %" PRIu32 "\n", args.nb_threads);
    
    int nb_prod_threads = ((int) args.nb_threads) - 1;
    int nb_of_file = argc;
    //int nb_cons_thread = 1;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0 , nb_prod_threads); 
    sem_init(&full, 0 , 0); 

    //Create the buffer
    buffer_info **buffer = create_shared_buffer(nb_of_file);

    // Open a file for each thread
    thread_parse_file(nb_prod_threads, nb_of_file, args, buffer);

    //Consume thread from the buffer
    thread_consume(buffer, args.output_stream);

    return 0;
}
    