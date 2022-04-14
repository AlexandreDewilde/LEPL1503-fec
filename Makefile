CC=gcc
CFLAGS=-Wall -Werror --std=gnu99
LIBS=-lcunit -lpthread -lm
INCLUDE_HEADERS_DIRECTORY=-Iheaders

fec: src/debug.c src/tinymt32.c src/system.c src/block.c main.c  # add your other object files needed to compile your program here. !! 
#The ordering is important !! if file_a.o depends on file_b.o, file_a.o must be placed BEFORE file_b.o in the list !
# this will run the following command: gcc -Wall -Werror -g -o kmeans src/distance.o other_object_filespresent_above.o ... -lcunit -lpthread
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS) -O3  


%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $< -O3

clean:
	rm -f src/*.o
	rm -f fec
	rm -f test_tinymt32

tests: src/debug.c src/tinymt32.c src/system.c tests/tools.c tests/test_tinymt32.c tests/test_system.c tests/tests.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) -o tests_suite -g $^ -lcunit
	./tests_suite -v
	
clean_tests:
	rm -f tests_suite

speed_tests: src/debug.c src/tinymt32.c src/system.c tests/tools.c tests/speed_tests/speed_tests.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) -o speed_tests $^ -O3
	./speed_tests

clean_speed_tests:
	rm -f speed_tests
# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests