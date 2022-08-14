#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "timer.h"

#include "p_queue.h"

/* 
typedef struct timer_thread
{
    pthread_t id;
    pthread_mutex_t lock

} timer_thread_t;

typedef struct timer_instance
{

} timer_inst_t;

timer_thread_t *timer_thread_init();
void timer_thread_join(timer_thread_t *timer_thread);
void timer_thread_force_stop_and_join(timer_thread_t *timer_thread);

timer_inst_t *set_timer(timer_thread_t *timer_thread);
void clear_timer(timer_thread_t *timer_thread, timer_inst_t* timer);
 */
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
