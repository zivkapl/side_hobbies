#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "simple_thread.h"


pthread_t thread_id = { 0 };


static void *myThreadFunc(void *param)
{
    uint64_t i;
    for(i = 0; i < (uint64_t)param; i++)
    {
        printf("from C thread. i = %llu\n", i);
        sleep(1);
    }

    return NULL;
}

void run(uint64_t param)
{
    pthread_create(&thread_id, NULL, myThreadFunc, (void *)param);
}


void join(void)
{
    pthread_join(thread_id, NULL);
}
