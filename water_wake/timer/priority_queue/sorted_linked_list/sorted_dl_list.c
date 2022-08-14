#include <stdlib.h>
#include <assert.h>

#include "dl_list.h"
#include "sorted_dl_list.h"

struct sorted_list
{
    dl_list_t *list;
    compare_func_t main_compare_func;
};

sorted_list_iter_t SortedListBegin(const sorted_list_t *s_list)
{
    sorted_list_iter_t s_iter = {0};

    assert(s_list);

    s_iter.iter = DLListBegin(s_list->list);

    #ifndef NDEBUG
        s_iter.list = s_list;
    #endif /* NDEBUG */

    return s_iter;
}

sorted_list_iter_t SortedListEnd(const sorted_list_t *s_list)
{
    sorted_list_iter_t s_iter = {0};

    assert(s_list);

    s_iter.iter = DLListEnd(s_list->list);

    #ifndef NDEBUG
        s_iter.list = s_list;
    #endif /* NDEBUG */

    return s_iter;
}

sorted_list_iter_t SortedListNext(sorted_list_iter_t s_iter)
{
    assert(s_iter.iter);

    s_iter.iter = DLListNext(s_iter.iter);

    return s_iter;
}

sorted_list_iter_t SortedListPrev(sorted_list_iter_t s_iter)
{
    assert(s_iter.iter);

    s_iter.iter = DLListPrev(s_iter.iter);

    return s_iter;
}

int SortedListIsSameIterator(const sorted_list_iter_t iter1, const sorted_list_iter_t iter2)
{
    assert(iter1.iter);
    assert(iter2.iter);

    return DLListIsSameIterator(iter1.iter, iter2.iter);
}

void *SortedListGetData(sorted_list_iter_t s_iter)
{
    assert(s_iter.iter);

    return DLListGetData(s_iter.iter);
}

size_t SortedListSize(const sorted_list_t *s_list)
{
    assert(s_list);

    return DLListSize(s_list->list);
}

int SortedListIsEmpty(const sorted_list_t *s_list)
{
    assert(s_list);

    return DLListIsEmpty(s_list->list);
}

sorted_list_iter_t SortedListRemove(sorted_list_iter_t s_iter)
{
    assert(s_iter.iter);

    s_iter.iter = DLListRemove(s_iter.iter);

    return s_iter;
}

int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to,
                    void *param, int (*action)(void *data, void *param))
{
    assert(from.iter);
    assert(to.iter);
    assert(to.list == from.list);
    assert(action);

    return DLListForEach(from.iter, to.iter, param, action);
}

sorted_list_t *SortedListCreate(compare_func_t compare_func)
{
    sorted_list_t *new_sorted_list = NULL;

    assert(compare_func);

    new_sorted_list = malloc(sizeof(sorted_list_t));

    if (NULL == new_sorted_list)
    {
        return NULL;
    }

    new_sorted_list->list = DLListCreate();

    if (NULL == new_sorted_list->list)
	{
		free(new_sorted_list);
		new_sorted_list = NULL;

		return NULL;
	}

    new_sorted_list->main_compare_func = compare_func;

    return new_sorted_list;
}

void *SortedListPopFront(sorted_list_t *s_list)
{
    void *data = NULL;

    assert(s_list);

    data = SortedListGetData(SortedListBegin(s_list));

    DLListPopFront(s_list->list);

    return data;
}

void *SortedListPopBack(sorted_list_t *s_list)
{
    void *data = NULL;

    assert(s_list);

    data = SortedListGetData(SortedListPrev(SortedListEnd(s_list)));

    DLListPopBack(s_list->list);

    return data;
}

void SortedListDestroy(sorted_list_t *s_list)
{
    assert(s_list);

    DLListDestroy(s_list->list);

    free(s_list);
    s_list = NULL;
}

sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from, sorted_list_iter_t to,
                                    void *data, compare_func_t compare_func)
{
    sorted_list_iter_t s_iter = {0};
    dll_iter_t dll_iter = NULL;

    assert(from.iter);
    assert(to.iter);
    assert(to.list == from.list);
    assert(compare_func);

    for (dll_iter = from.iter;
            0 == DLListIsSameIterator(dll_iter, to.iter);
            dll_iter = DLListNext(dll_iter))
   {
        int ret_value = compare_func(DLListGetData(dll_iter), data);

        if (0 != ret_value)
        {
            break;
        }
    }

    s_iter.iter = dll_iter;

    return s_iter;
}

sorted_list_iter_t SortedListFind(sorted_list_t *s_list, sorted_list_iter_t from, sorted_list_iter_t to, void *data)
{
    sorted_list_iter_t s_iter = from;

    assert(s_list->main_compare_func);
    assert(from.iter);
    assert(to.iter);
    assert(to.list == from.list);

    for  (; 0 == SortedListIsSameIterator(s_iter, to);
                s_iter = SortedListNext(s_iter))
	{
		int ret_val = s_list->main_compare_func(DLListGetData(s_iter.iter), data);

		if (0 <= ret_val)
        {
            if (0 < ret_val)
            {
              s_iter = to;
            }

            break;
        }
	}

    return s_iter;
}

sorted_list_iter_t SortedListInsert(sorted_list_t *s_list, void *data)
{
    sorted_list_iter_t s_iter = {0};

    assert(s_list);

    for (s_iter = SortedListBegin(s_list);
                0 == SortedListIsSameIterator(s_iter, SortedListEnd(s_list));
               s_iter = SortedListNext(s_iter))
	{
		int ret_val = s_list->main_compare_func(DLListGetData(s_iter.iter), data);

		if (0 < ret_val)
		{
            /* place found */
            break;
		}
	}

    s_iter.iter = DLListInsert(s_iter.iter, data);

    if (NULL == s_iter.iter)
	{
		s_iter = SortedListEnd(s_list);
	}

    #ifndef NDEBUG
        s_iter.list = s_list;
    #endif /* NDEBUG */

    return s_iter;
}

void SortedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
    sorted_list_iter_t dest_iter = {0};
    sorted_list_iter_t src_iter = {0};

    assert(dest);
    assert(src);

    dest_iter = SortedListBegin(dest);
    src_iter = SortedListBegin(src);

    while (!SortedListIsEmpty(src) &&
            !SortedListIsSameIterator(dest_iter, SortedListEnd(dest)))
    {
        int ret_val = dest->main_compare_func(SortedListGetData(dest_iter),
                                          SortedListGetData(src_iter));
        if (0 <= ret_val)
		{
            sorted_list_iter_t temp = SortedListNext(src_iter);;
            DLListSplice(dest_iter.iter, src_iter.iter, DLListNext(src_iter.iter));
            src_iter = temp;
		}

        dest_iter = SortedListNext(dest_iter);
    }

    if (!SortedListIsEmpty(src))
    {
        DLListSplice(dest_iter.iter, src_iter.iter, DLListEnd(src->list));
    }
}
