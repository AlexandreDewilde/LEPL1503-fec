For better visibilty please open this file with a modern MarkDown(.md) reader.

# LEPL1503-2022 - GROUPE W4
Below, you will find everything you need know about our project from the course LEPL1503.

## 1. Our Objective

Our goal is, given a Python code which does resolve the complicated problem of
the use of error-correcting codes (*codes correcteurs d'erreurs*), we have *to do a C version of the code/program*. 
The C program must do the exact same thing as the Python program but must be faster. In addition, we must do a parallelize the code (using Threads). More to that our C program must run well on Raspberry Pi, with an energy consumption that is averagely good.

For more specifications, check out [here](https://moodle.uclouvain.be/pluginfile.php/334805/mod_resource/content/3/lepl1503_project_statement.pdf).

### 1. 1 User inputs

The user has a certain power over the perfomance of the program since she/he has to determine the following arguments : 

| Argument | Meaning |
| :------- | :------ |
| **input_dir**| The directory of the folder containing all binary files|
| **-n** n_threads (default: 4) | The number of compute threads that are used to solve linear system of matrices |
| **-v** | If the user want to run the program with verbose, meaning printing in the terminal parts of the program|
| **-f** output_file (by default, we write to the standard output) | The path to the file for writing the result |

## 2. Folder Organisation

Our project folder is like follow:
```
|hamlet/
|headers/
|samples/
|src/
|tests/
|main.c
|Makefile
|README
```
* Hamlet, a folder containing a sample file that is the tragedy of hamlet with lost words and redudancy symbols
* All files that end with **.h** will be in `headers` folder. As the name of the folder indicate, this will contain our custom headers used in the project. 
* `samples` folder contains exaples of input binary files. 
* All files in `src` folder are source files **.c**, that implement the functions defined in their corresponding headers in `headers`.
* In `tests` folder, there are
    * `headers` folder which contains files that end with **.h**, used to tests our program.
    * `results` folder containing **.bin** files, which group the correct answer for the calculations for each file given in the samples folder.
    * `speed_tests` folder containing a source file **.c** , that tests the speed of our functions and systems.
    * Tests files **.c**, that implement the testing functions defined in their corresponding headers in `headers`.

* `main.c` is our main file.


## 3. System Architecture
In order to keep our code easily maintanable and understable, we had to use various software design techniques :

### 3. 1 Modular Programming
It is a software design technique whose aim is to separate the functionalites of a whole program into different (nearly) independent modules.

In our project we have about 6 modules : 
 
| Module            | Purpose                                                 | Dependency On Other Modules |
| ----------------- | :------------------------------------------------------ | ---------: |
| block   | All functions related to blocks parsing and writing | Yes| 
| debug   | All functions used to debug code in verbose mode | No | 
| program | The program function | Yes|
| system  | All functions related to linear system solving | Yes|
| tinymt32| Functions related to random number generations | No |
| utils   | Some utils function used in the program | Yes|



Note that these modules are mostly dependent. It was the intention, because the main goal was to have a program that's easily understable and maintanable rather than completely independent.

### 3. 2 Data abstraction Programming
The idea is to use structures whenever we can use them. Given that C doesn't support Object Oriented Paradigm we had to use structures in order to have a certain level of abstraction, hence improving the coding experience.

Below are the structures that we used in our program. For more detailed specifications we recommend to check the `headers` file in which they are initiated.

#### 3. 2. 1 Structure of a block [headers/block.h]
Mathematical points with a certain dimension are represented as below : 

```c
typedef struct {
    uint32_t block_size, word_size;
    uint8_t *message;
} block_t;

typedef struct {
    uint32_t seed, block_size, word_size, redudancy;
    uint64_t file_size, message_size;

} file_info_t;

typedef struct {
    char *filename;
    FILE *file;
} file_thread_t;

typedef struct {
    uint8_t *file_data;
    char *filename;
    uint64_t file_size;
    uint64_t message_size;
    block_t *blocks;
    uint64_t nb_blocks;
    uint32_t remaining;
    bool uncomplete_block;

} output_infos_t;
```


#### 3. 2. 2 User Input Arguments [headers/utils.h]
  

```c
typedef struct
{
    DIR *input_dir;
    char input_dir_path[PATH_MAX];
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;
```

### 3. 3 Concurrent Programming
For the multi threading, we used POSIX threads. Our goal was to separate the code into 3 main parts of the program in order to have a real sense of parallelism. Our three main parts are :

* Retrieving file from the input folder of the user. There is only 1 thread executing this part, **folder_producer**. This thread is gets the inputs files and puts them, one by one in the  **the first shared buffer**.

* Retrieving the *lost words* by resolving the linear system equations. The number of threads executing this part is **-n**, corresponding to the user input, other wise it's 4 by default. These threads will be called **producer**. Each producer treats one file and puts the result in the **the second shared buffer**.

* Writing the result to the output file or the terminal( in case the output file not given). There is only 1 thread executing this part, **consumer**. This thread gets the data from the *seconde shared buffer* and writes it in the outpout file or the terminal.

#### 3. 3. 1 Design

The design we used throughout our project is the `Consumer/Producer Pattern`. This design pattern means that some threads produce while others consume what the producers produced. This design pattern is used twice in our project as explained previously.
We used semaphores and mutex to handle communication and exclusivity respectfull in our project. 

In order to ensure that we consume the least memory possible, we :
* Decreased the number of memory allocations: The allocation tion of memory on the heap is something that takes time time, and some embedded systems may have their RAM limited. Therefore, the number of allocation has been reduced.
* We used the  pointer arithmetic in order to prevent extra allocations on the heap.
* We avoided allocating memory while writing in the output file and replace some functions which allocated additional memory by functions in place

For more precision and example, check out the report file.

#### 3. 3. 2 Data Structure 

The data structure used for holding the result is a circular fixed size buffer. The size of the buffer is equal to the user input number of threads. We choose a fixed size because of various reasons, the main reason was that since, a Raspberry Pi is an embedded computer with (1GB RAM), for big computations a varying size buffer may cause a heap overflow. Therefore with our data structure we are more than sure that it won't be a problem.

### 3. 4 Image :
[To be changed]

![alt text](../lepl1503-2022-skeleton-group-w4/Architecture_du_Projet_3.png "Design view").

A system architecture image can be found in project folder.


### 3. 4 Error Handling

Error handling is a big task in C programs, since there's no concept of exceptions. However one can use *gotos*, but they are complex to manage so we opt for checking returned signals by functions. The disadvantage of the chosen option is that the code is longer and is filled with a lot of ifs conditions.

The main aim with our error handling was that incase of an error the program must end correctly, by freeing all the resources and signaling all the other threads.

In order to manage erros we used only one way of handling them, and we called the errors  **Critical Errors**: When these errors occur the execution of the current function ends right away. Before exiting the function we print a message about where the error occured or what is the exact error.

Here's an pseudo code example showing how we treat **Critical Errors**.

```C
if ((err = pthread_create(&cons, NULL, (void *) consumer, NULL)) != 0) {
    fprintf(stderr, "Failed to create consumer thread, error code : %d\n", err);
    exit(EXIT_FAILURE);
};
```
OR
```C
uint8_t **coefs = (uint8_t **)  malloc(nrs * sizeof(uint8_t *) + nrs * nss);
if (!coefs) {
    fprintf(stderr, "Failed to allocate memory for coefficients\n");
    exit(EXIT_FAILURE);
}

```



## 4. Code Execution
In order to executing our program, you just need to execute the following command in the project directory : 
```
> make fec
```
Once the program is compiled then you can run the runnable `fec` like this ;
```
> ./fec [arguments] 
```
For [arguments]: see 1.1 above if needed

## 5. Testing

The tests are in the `tests/` directory. 

### Goal 

The main goal of our test is to check if the modules we defined in `headers` and implimentated in `src` work as they should. For that reason, most files in `src` will have a test. It is important to note that it is possible that one `src` file depends on another file for it be fully functional, a dependency table is available in section [3.1].

We also use the python code given to test the output of the whole function and the speed of the main.c if we increase the number of threads on multi core systems.

### Requirements

We use `CUnit` to test our functions. In order to run our tests, this library must be installed, here's the [link](https://sourceforge.net/projects/cunit/files/) to the sourceforge site.

### Execution
Once installed, open terminal, go to the project's directory and execute,
* To test our program (results and functions)

```
make tests
```
* To test the speed of our program
```
make speed_tests
```

All the tests will be executed and a failure report will be displayed for each test.

## 6. Makefile

There is Makefile in the root directory, this makefile contains the following commands to facilitate the compilation of various C files. Note that all of the following commands are to be run in the terminal ( prefably on a Linux Operating System).

### Requirements 

We used various in built commands for our makefile, one may need to install some of them :

* **CPP Check** : A static tool for bugs and undefined behaviour detection. [Install](http://cppcheck.sourceforge.net/)

* **Valgrind** : A dynamic analysis tool for memory leak detection, threading bugs etc. [Install](https://riptutorial.com/valgrind/example/32345/installation-or-setup)


| command                 | functionalities                                             |
| :-----------------      | :---------------------------------------------------------- |
| make fec                | Compile the fec executable object.|
| make tests              | Execute all our tests with failure reports.|
| make speed_tests        | Execute all our speed tests|
| make clean              | Cleans `fec` executable and all source objects in the root directory and it's children directories.|
| make clean_tests        | Cleans all the testing objects and executables in the root directory and it's children directories except, `fec` executable. |
| make clean_speed_tests  | Cleans all the speed testing objects and executables in the root directory and it's children directories except, `fec` executable.  |
