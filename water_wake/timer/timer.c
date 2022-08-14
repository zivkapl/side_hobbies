#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


typedef enum priorities
{
    LOW,
    MEDIUM,
    HIGH
} priorities_t;

struct timer_service
{
    pthread_t id;
    p_queue_t *queue;
};

struct timer_instance
{
    void *(*callback)(void *param);
    void *param;
    time_t time_to_ring;
    timer_states_t is_active;
    priorities_t priority;
};

static void *myThreadFunc(void *param);
static int compare_func(const void *data1, const void *data2);

/* // TODO: CHECK */
static int compare_func(const void *data1, const void *data2)
{
    return ((timer_instance_t *)data1)->priority -
            ((timer_instance_t *)data2)->priority;
}


timer_service_t *timer_thread_init(void)
{
    timer_service_t* new_service = NULL;
    p_queue_t *new_queue = NULL;
    pthread_t new_thread = 0;
    int res = 0;
    
    new_service = (timer_service_t*)malloc(sizeof(timer_service_t));
    if (!new_service)
    {
        return NULL;
    }

    new_queue = PQueueCreate(compare_func);
    if (!new_queue)
    {
        free(new_service);
        new_service = NULL;

        return NULL;
    }

    /* init */
    memset(new_service, 0 , sizeof(timer_service_t));

    new_service->queue = new_queue;

    res = pthread_create(&new_thread, NULL, myThreadFunc, (void *)(new_service->queue));
    if (res)
    {
        free(new_service);
        new_service = NULL;

        PQueueDestroy(new_queue);
        new_queue = NULL;

        return NULL;
    }

    new_service->id = new_thread;

    return new_service;
}

void timer_thread_destroy(timer_service_t* timer_service)
{
    if (!timer_service)
    {
        return;
    }


}

static void *myThreadFunc(void *param)
{
    p_queue_t *queue = (p_queue_t *)param;
    timer_instance_t *task = (timer_instance_t *)PQueueDequeue(queue);

    if (task->is_active && (NULL) < task->time_to_ring)
    {
        PQueueEnqueue(queue, (void *)task);
        /* // TODO: error handling */
    }
    else
    {
        if (task->is_active)
        {
            task->callback(task->param);
        }
     
        /* free task object */
    }

    return NULL;
}
