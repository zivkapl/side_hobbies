#ifndef __OL113_DOUBLY_LINKED_LIST_H__
#define __OL113_DOUBLY_LINKED_LIST_H__

#include <stddef.h> /* size_t */

/**************************************************************
 * version 1.0                                                *
 *************************************************************/

typedef struct list dl_list_t;
typedef struct node *dll_iter_t;

dl_list_t *DLListCreate(void);
void DLListDestroy(dl_list_t *list);

dll_iter_t DLListInsert(dll_iter_t iter, void *data);
dll_iter_t DLListRemove(dll_iter_t iter);

dll_iter_t DLListPushFront(dl_list_t *list, void *data);
dll_iter_t DLListPushBack(dl_list_t *list, void *data);
dll_iter_t DLListPopFront(dl_list_t *list);
dll_iter_t DLListPopBack(dl_list_t *list);

size_t DLListSize(const dl_list_t *list);
int DLListIsEmpty(const dl_list_t *list);

void *DLListGetData(dll_iter_t iter);

dll_iter_t DLListBegin(const dl_list_t *list);
dll_iter_t DLListEnd(const dl_list_t *list);
dll_iter_t DLListNext(dll_iter_t iter);
dll_iter_t DLListPrev(dll_iter_t iter);
int DLListIsSameIterator(const dll_iter_t iter1, const dll_iter_t iter2);

dll_iter_t DLListFind(dll_iter_t from_iter, dll_iter_t to_iter, void *find_data, int (*cmp_func)(const void *data, const void *find_data));
int DLListForEach(dll_iter_t from_iter, dll_iter_t to_iter, void *param, int (*oper_func)(void *data, void *param));

int DLListMultiFind(const dl_list_t *list, void *find_data, int (*cmp_func)(const void *data, const void *find_data), dl_list_t *output_list);

void DLListSplice(dll_iter_t dest_where, dll_iter_t src_iter_from, dll_iter_t src_iter_to);

#endif /* __OL113_DOUBLY_LINKED_LIST_H__ */