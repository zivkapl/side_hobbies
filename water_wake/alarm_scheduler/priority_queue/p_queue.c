#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() */

#include "p_queue.h"
#include "sorted_dl_list.h"

enum exit_status {SUCCESS, FAIL};

struct p_queue
{
    sorted_list_t *s_list;
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

    return new_p_queue;
}

void PQueueDestroy(p_queue_t *p_queue)
{
    assert(p_queue);

    SortedListDestroy(p_queue->s_list);

    free(p_queue);
    p_queue = NULL;
}

int PQueueEnqueue(p_queue_t *p_queue, void *data)
{
    sorted_list_iter_t new_iter;

    assert(p_queue);

    new_iter = SortedListInsert(p_queue->s_list, data);

    if (SortedListIsSameIterator(new_iter, SortedListEnd(p_queue->s_list)))
    {
        return FAIL;
    }

    return SUCCESS;
}

void *PQueueDequeue(p_queue_t *p_queue)
{
    assert(p_queue);

    return SortedListPopFront(p_queue->s_list);
}

void *PQueuePeek(p_queue_t *p_queue)
{
    assert(p_queue);

    return SortedListGetData(SortedListBegin(p_queue->s_list));
}

int PQueueIsEmpty(const p_queue_t *p_queue)
{
    assert(p_queue);
    return SortedListIsEmpty(p_queue->s_list);
}

size_t PQueueSize(const p_queue_t *p_queue)
{
    assert(p_queue);
    return SortedListSize(p_queue->s_list);
}

void PQueueClear(p_queue_t *p_queue)
{
    assert(p_queue);

    while (!PQueueIsEmpty(p_queue))
    {
        PQueueDequeue(p_queue);
    }
}

void *PQueueErase(p_queue_t *p_queue, void *data,
  					int (*IsMatch)(const void *data1, const void *data2))
{
	sorted_list_iter_t found_iter = {0};
	void *found_data = NULL;

    assert(p_queue);
    assert(IsMatch);

	found_iter = SortedListFindIf(SortedListBegin(p_queue->s_list),
								  SortedListEnd(p_queue->s_list), data, IsMatch);

	if (SortedListIsSameIterator(found_iter, SortedListEnd(p_queue->s_list)))
	{
		return NULL;
	}

	found_data = SortedListGetData(found_iter);
	SortedListRemove(found_iter);

	return found_data;
}
