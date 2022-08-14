#ifndef __OL113_P_QUEUE_H__
#define __OL113_P_QUEUE_H__

/********************************************/
/* Priority Queue API                       */
/* version 2.0                              */
/********************************************/

#include <stddef.h> /* size_t */

typedef struct p_queue p_queue_t;
typedef int (*priority_cmp_t)(const void *data1, const void *data2);

p_queue_t *PQueueCreate(priority_cmp_t priority_cmp);

void PQueueDestroy(p_queue_t *p_queue);

int PQueueEnqueue(p_queue_t *p_queue, void *data);
void *PQueueDequeue(p_queue_t *p_queue);
void *PQueuePeek(p_queue_t *p_queue);

int PQueueIsEmpty(const p_queue_t *p_queue);
size_t PQueueSize(const p_queue_t *p_queue);

void PQueueClear(p_queue_t *p_queue);
void *PQueueErase(p_queue_t *p_queue, void *data,
                  int (*IsMatch)(const void *data1, const void *data2));

#endif /* __OL113_P_QUEUE_H__ */