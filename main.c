#include "program.h"


int main(int argc, char *argv[])
{
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    int ret =  program(argc, argv);

    DEBUG_end_time(&end);
    DEBUG_PRINT_TIME_USED(DEBUG_get_delta_time(start,end));

    return ret;
}