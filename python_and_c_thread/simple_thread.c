#include <stdio.h>
#include <pthread.h>

#include "simple_thread.h"


pthread_t thread_id = { 0 };


static void *myThreadFunc(void *param)
{
    int i;
    for(i = 0; i < 10000; i++)
    {
        printf("from C thread. i = %d\n", i);\
    }

    return NULL;
}

void run(void)
{
    pthread_create(&thread_id, NULL, myThreadFunc, NULL);
}


void join(void)
{
    pthread_join(thread_id, NULL);
}
