CC=gcc
CFLAGS=-Wall -Werror -Wextra  --std=gnu99
LIBS=-lcunit -lpthread -lm
INCLUDE_HEADERS_DIRECTORY=-Iheaders
PROGRAM_FILES=src/program.c src/debug.c src/tinymt32.c src/system.c src/block.c 
THREAD_PROGRAM_FILES=src/debug.c src/tinymt32.c src/system.c src/block.c src/shared_buffer.c src/producer.c src/consumer.c src/prod_cons_program.c
TEST_FILES=tests/test_block.c tests/test_program.c tests/test_tinymt32.c tests/test_system.c

fec: $(PROGRAM_FILES) main.c  # add your other object files needed to compile your program here. !! 
#The ordering is important !! if file_a.o depends on file_b.o, file_a.o must be placed BEFORE file_b.o in the list !
# this will run the following command: gcc -Wall -Werror -g -o kmeans src/distance.o other_object_filespresent_above.o ... -lcunit -lpthread
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS) -O3  


%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $< -O3

clean:
	rm -f src/*.o
	rm -f fec
	rm -f test_tinymt32

tests: $(PROGRAM_FILES) $(TEST_FILES) tests/tools.c tests/tests.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS)  -o tests_suite -g $^ -lcunit $(LIBS)
	./tests_suite -v

thread_tests:  $(THREAD_PROGRAM_FILES) $(TEST_FILES) tests/tools.c tests/tests.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS)  -o tests_suite -g $^ -lcunit $(LIBS)
	./tests_suite -v
	
clean_tests:
	rm -f tests_suite

speed_tests: src/debug.c src/tinymt32.c src/system.c tests/tools.c tests/speed_tests/speed_tests.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o speed_tests $^ -O3
	./speed_tests

clean_speed_tests:
	rm -f speed_tests
# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests