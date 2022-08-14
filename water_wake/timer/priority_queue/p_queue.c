#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() */
#include <pthread.h> /* mutex */

#include "p_queue.h"
#include "sorted_dl_list.h"

enum exit_status { SUCCESS, FAIL };

struct p_queue
{
    sorted_list_t *s_list;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond_var;
};

p_queue_t *PQueueCreate(priority_cmp_t priority_cmp)
{
    p_queue_t *new_p_queue = NULL;

    assert(priority_cmp);

    new_p_queue = malloc(sizeof(p_queue_t));

    if (NULL == new_p_queue)
    {
        return NULL;
    }

    new_p_queue->s_list = SortedListCreate(priority_cmp);

    if (NULL == new_p_queue->s_list)
    {
        free(new_p_queue);
        new_p_queue = NULL;

        return NULL;
    }

    
    if (pthread_mutex_init(new_p_queue->mutex, NULL))
    {
        free(new_p_queue);
        new_p_queue = NULL;

        SortedListDestroy(new_p_queue->s_list);
        new_p_queue->s_list = NULL;

        return NULL;
    }


    if (pthread_cond_init(new_p_queue->cond_var, NULL))
    {
        free(new_p_queue);
        new_p_queue = NULL;

        SortedListDestroy(new_p_queue->s_list);
        new_p_queue->s_list = NULL;

        pthread_mutex_destroy(new_p_queue->mutex);
        new_p_queue->mutex = NULL;

        return NULL;
    }


    return new_p_queue;
}

void PQueueDestroy(p_queue_t *p_queue)
{
    assert(p_queue);

    SortedListDestroy(p_queue->s_list);
    p_queue->s_list = NULL;

    pthread_mutex_destroy(p_queue->mutex);
    p_queue->mutex = NULL;

    pthread_cond_destroy(p_queue->cond_var);
    p_queue->mutex = NULL;

    free(p_queue);
    p_queue = NULL;
}

int PQueueEnqueue(p_queue_t *p_queue, void *data)
{
    sorted_list_iter_t new_iter;

    assert(p_queue);

    pthread_mutex_lock(p_queue->mutex);

    new_iter = SortedListInsert(p_queue->s_list, data);

    if (SortedListIsSameIterator(new_iter, SortedListEnd(p_queue->s_list)))
    {
        pthread_mutex_unlock(p_queue->mutex);
        return FAIL;
    }

    pthread_mutex_unlock(p_queue->mutex);

    pthread_cond_broadcast(p_queue->cond_var);

    return SUCCESS;
}

void *PQueueDequeue(p_queue_t *p_queue)
{
    void *data = NULL;

    assert(p_queue);
    
    pthread_mutex_lock(p_queue->mutex);

    while(PQueueIsEmpty(p_queue))
    {
        pthread_cond_wait(p_queue->cond_var, p_queue->mutex);
    }

    data = SortedListPopFront(p_queue->s_list);

    pthread_mutex_unlock(p_queue->mutex);

    return data;
}

int PQueueIsEmpty(const p_queue_t *p_queue)
{
    assert(p_queue);
    
    return SortedListIsEmpty(p_queue->s_list);
}
