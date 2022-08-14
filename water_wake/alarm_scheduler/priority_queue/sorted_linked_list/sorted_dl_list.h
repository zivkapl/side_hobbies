#ifndef __OL113_SORTED_L_LIST_H__
#define __OL113_SORTED_L_LIST_H__

/******************************************/
/* Sorted Linked List API                 */
/* Version 5                              */ 
/******************************************/

#include <stddef.h> /* size_t */
#include "dl_list.h"

typedef struct sorted_list sorted_list_t;
typedef struct sorted_list_iter sorted_list_iter_t;
typedef int (*compare_func_t)(const void *data1, const void *data2);

struct sorted_list_iter
{
    dll_iter_t iter;
    #ifndef NDEBUG
        const sorted_list_t *list;
    #endif /* NDEBUG */
};

sorted_list_t *SortedListCreate(compare_func_t compare_func);

/* O(n) */
void SortedListDestroy(sorted_list_t *s_list);

/* O(n) */
size_t SortedListSize(const sorted_list_t *s_list);

/* O(1) */
int SortedListIsEmpty(const sorted_list_t *s_list);

/* O(1) */
sorted_list_iter_t SortedListBegin(const sorted_list_t *s_list);
sorted_list_iter_t SortedListEnd(const sorted_list_t *s_list);
sorted_list_iter_t SortedListNext(sorted_list_iter_t s_iter);
sorted_list_iter_t SortedListPrev(sorted_list_iter_t s_iter);
int SortedListIsSameIterator(const sorted_list_iter_t iter1, const sorted_list_iter_t iter2);
void *SortedListGetData(sorted_list_iter_t s_iter);

/* O(n) */
sorted_list_iter_t SortedListInsert(sorted_list_t *s_list, void *data);

/* O(1) */
sorted_list_iter_t SortedListRemove(sorted_list_iter_t s_iter);
void *SortedListPopFront(sorted_list_t *s_list);
void *SortedListPopBack(sorted_list_t *s_list);

/* O(n) */
/* return user's func return value. If not 0 stops the iteration */
int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to,
											void *param, int (*action)(void *data, void *param));
/* O(n) */
void SortedListMerge(sorted_list_t *dest, sorted_list_t *src);

/* O(n) */
sorted_list_iter_t SortedListFind(sorted_list_t *s_list, sorted_list_iter_t from, sorted_list_iter_t to, void *data);

/* O(n) */
sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from, sorted_list_iter_t to,
									void *data, compare_func_t compare_func);

#endif /* __OL113_SORTED_L_LIST_H_ */