#include <stdio.h>
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

typedef struct timer_resources
{
    pthread_mutex_t lock;
    p_queue_t *queue;
} timer_resources_t;

struct timer_thread
{
    pthread_t id;
    timer_resources_t resources;
};

struct timer_instance
{
    void *(*callback)(void *param);
    void *param;
    time_t time_to_ring;
    timer_states_t is_active;
    priorities_t priority;
};

/* // TODO: CHECK */
static int compare_func(const void *data1, const void *data2)
{
    return ((timer_instance_t *)data1)->priority -
            ((timer_instance_t *)data2)->priority;
}


timer_thread_t *timer_thread_init(void)
{
    timer_thread_t* new_service = NULL;
    p_queue_t *new_queue = NULL;
    pthread_mutex_t* new_lock = NULL;
    pthread_t new_thread = 0;
    int res = 0;
    
    new_service = (timer_thread_t*)malloc(sizeof(timer_thread_t));
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

    res = pthread_mutex_init(new_lock, NULL);
    if (res)
    {
        free(new_service);
        new_service = NULL;

        PQueueDestroy(new_queue);
        new_queue = NULL;

        /* // TODO: send task that closes the thread */
        pthread_join(thread_id, NULL);

        return NULL;
    }

    /* init */
    memset(new_service, 0 , sizeof(timer_thread_t));

    new_service->resources.queue = new_queue;
    new_service->resources.lock= new_lock;

    res = pthread_create(&new_thread, NULL, myThreadFunc, (void *)(&new_service->resources));
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


static void *myThreadFunc(void *param)
{
    timer_resources_t *resources = (timer_resources_t *)param;
    pthread_mutex_lock(resources->lock);
    
    pthread_mutex_unlock(resources->lock);



    return NULL;
}



void join(void)
{
    pthread_join(thread_id, NULL);
}
 */
