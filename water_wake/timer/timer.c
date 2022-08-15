#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "timer.h" /* header file of this source file */
#include "p_queue.h"

#define true   1
#define false  0
#define bool  int

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

struct timer_task
{
    void *(*callback)(void *param);
    void *param;
    time_t time_to_ring;
    bool is_active;
    priorities_t priority;
};

static void *myThreadFunc(void *param);
static int compare_func(const void *data1, const void *data2);
static timer_task_t *_set_timer(timer_service_t *timer_service,
                        uint64_t seconds_from_now,
                        void *(*callback)(void *param), 
                        void *param,
                        priorities_t priority);
static bool callback_wrapper(void *(*callback)(void *param), void *param);
static void _timer_service_join(timer_service_t *timer_service, priorities_t priority);
static void _timer_service_destroy(timer_service_t* timer_service);

static int compare_func(const void *data1, const void *data2)
{
/* // TODO: CHECK */
    return ((timer_task_t *)data1)->priority -
            ((timer_task_t *)data2)->priority;
}

static bool callback_wrapper(void *(*callback)(void *param), void *param)
{
    if (callback)
    {
        callback(param);
        return true;
    }

    return false;
}

timer_service_t *timer_service_init(void)
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

    res = pthread_create(&new_thread, NULL, myThreadFunc, (void *)(new_service));
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

static void _timer_service_destroy(timer_service_t* timer_service)
{    
    if (!timer_service)
    {
        return;
    }

    while (!PQueueIsEmpty(timer_service->queue))
    {
        free(PQueueDequeue(timer_service->queue));
    }

    PQueueDestroy(timer_service->queue);
    timer_service->queue = NULL;

    free(timer_service);
    timer_service = NULL;
}

static void *myThreadFunc(void *param)
{
    p_queue_t *queue = ((timer_service_t *)param)->queue;
    bool keep_running = true;

    while(keep_running)
    {
        timer_task_t *task = (timer_task_t *)PQueueDequeue(queue);

        if (time(NULL) >= task->time_to_ring)
        {
            if (task->is_active)
            {
                keep_running = callback_wrapper(task->callback, task->param);
            }
        
            /* free task object */
            free(task);
            task = NULL;
        }
        else
        {
            if (EXIT_SUCCESS != PQueueEnqueue(queue, (void *)task))
            {
                ;    /* // TODO: log error */
            }
        }
    }

    return NULL;
}

static timer_task_t *_set_timer(timer_service_t *timer_service,
                        uint64_t seconds_from_now,
                        void *(*callback)(void *param), 
                        void *param,
                        priorities_t priority)
{
    timer_task_t *new_task = (timer_task_t *)malloc(sizeof(timer_task_t));
    if (!new_task)
    {
        return NULL;
    }

    new_task->callback = callback;
    new_task->is_active = true;
    new_task->param = param;
    new_task->priority = priority;
    new_task->time_to_ring = time(NULL) + seconds_from_now;

    if (EXIT_SUCCESS != PQueueEnqueue(timer_service->queue, (void *)new_task))
    {
        ;    /* // TODO: log error */
        free(new_task);
        new_task = NULL;

        return NULL;
    }

    return new_task;
}


timer_task_t *set_timer(timer_service_t *timer_service,
                        uint64_t seconds_from_now,
                        void *(*callback)(void *param), 
                        void *param)
{
    assert(timer_service);
    assert(callback);
    
    return _set_timer(timer_service, seconds_from_now, callback, param, MEDIUM);
}

void deactivate_timer(timer_task_t* timer)
{
    if (!timer)
    {
        return;
    }

    timer->is_active = false;
}

static void _timer_service_join(timer_service_t *timer_service, priorities_t priority)
{
    assert(timer_service);

    if (!_set_timer(timer_service, 0, NULL, NULL, priority))
    {
        ;    /* // TODO: handle error */
    }

    if (pthread_join(timer_service->id, NULL))
    {
        ;    /* // TODO: handle error */
    }
}

void timer_service_force_stop_and_join(timer_service_t *timer_service)
{
    assert(timer_service);

    _timer_service_join(timer_service, HIGH);
    _timer_service_destroy(timer_service);

}

void timer_service_wait_and_join(timer_service_t *timer_service)
{
    assert(timer_service);

    _timer_service_join(timer_service, LOW);
    _timer_service_destroy(timer_service);

}
