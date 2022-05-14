#include "program.h"


int main(int argc, char *argv[])
{
    struct timeval start;
    struct timeval end;
    DEBUG_start_time(&start);
    
    int ret =  program(argc, argv);

    DEBUG_end_time(&end);
    double used_time = DEBUG_get_delta_time(start,end);
    DEBUG_PRINT_TIME_USED(used_time);
    printf("The used time is equal to : %f", (double) end.tv_usec - start.tv_usec);

    return ret;
}