#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "timer.h" /* header file of this source file */
#include "p_queue.h"

typedef enum timer_states
{
    OFF = 0,
    ON
} timer_states_t;

struct timer_thread
{
    pthread_t id;
    pthread_mutex_t lock;
    p_queue_t *queue;
};

struct timer_instance
{
    void *(*callback)(void *param);
    void *param;
    time_t time_to_ring;
    timer_states_t is_active;
};


/* 
static void *myThreadFunc(void *param)
{
pthread_t thread_id = { 0 };
    int i;
    for(i = 0; i < (uint64_t)param; i++)
    {
        printf("from C thread. i = %d\n", i);
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
 */
