#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() */

#include "dl_list.h"

typedef struct node
{
    struct node *next;
    struct node *prev;
    void *data;
    
} node_t;

struct list
{
    node_t *first;
    node_t *last;
};

dl_list_t *DLListCreate(void)
{	
	dl_list_t *new_list = NULL;

	node_t *first_dummy = NULL;
	node_t *last_dummy  = NULL;

	new_list = malloc(sizeof(dl_list_t));

	if (NULL == new_list)
	{
		return NULL;
	}

	first_dummy = malloc(sizeof(node_t));
	last_dummy = malloc(sizeof(node_t));

	if (NULL == first_dummy ||
		NULL == last_dummy)
	{
		free(first_dummy);
		free(last_dummy);
		free(new_list);

		first_dummy = NULL;
		last_dummy = NULL;
		new_list = NULL;
			
		return NULL;
	}
	
	new_list->first = first_dummy;
	new_list->last = last_dummy;

	first_dummy->data = (void *)new_list;
	first_dummy->prev = NULL;
	first_dummy->next = last_dummy;

	last_dummy->data = (void *)new_list;
	last_dummy->prev = first_dummy;
	last_dummy->next = NULL;

	return new_list;	
}

void DLListDestroy(dl_list_t *list)
{
	dll_iter_t current = NULL;
	dll_iter_t end_of_list = NULL;
	dll_iter_t next_iter = NULL;

	assert(list);

	current = DLListBegin(list);
	end_of_list = DLListEnd(list);

	while (current != end_of_list)
	{
		next_iter = current->next;
		free(current);
		current = next_iter;
	}
	
	free(list->first);
	free(list->last);

	free(list);
	list = NULL;
}

size_t DLListSize(const dl_list_t *list)
{
	
	size_t counter = 0;

	dll_iter_t iter = NULL;
	dll_iter_t list_end = NULL;

	assert(list);

	iter = DLListBegin(list);
	list_end = DLListEnd(list);


	for ( ; iter != list_end; iter = DLListNext(iter))
    {
        counter++;
    }

	return counter;
}

int DLListIsEmpty(const dl_list_t *list)
{
	assert(list);
	
	return (DLListIsSameIterator(DLListBegin(list), DLListEnd(list)));
}

dll_iter_t DLListBegin(const dl_list_t *list)
{
	assert(list);
	
	return (list->first)->next;
}

dll_iter_t DLListEnd(const dl_list_t *list)
{
	assert(list);
	
	return list->last;
}

dll_iter_t DLListNext(dll_iter_t iter)
{
	assert(iter);

	return iter->next;
}

dll_iter_t DLListPrev(dll_iter_t iter)
{
	assert(iter);

	return iter->prev;
}

void *DLListGetData(dll_iter_t iter)
{
	assert(iter);

	return iter->data;
}

int DLListIsSameIterator(const dll_iter_t iter1, const dll_iter_t iter2)
{
	assert(iter1);
	assert(iter2);

	return (iter1 == iter2);
}

dll_iter_t DLListPushFront(dl_list_t *list, void *data)
{
	assert(list);
	
	return DLListInsert(DLListBegin(list), data);
}

dll_iter_t DLListPushBack(dl_list_t *list, void *data)
{
	assert(list);
	
	return DLListInsert(DLListEnd(list), data);
}

dll_iter_t DLListInsert(dll_iter_t iter, void *data)
{
	node_t *new_node = NULL;

	assert(iter);

	new_node = malloc(sizeof(node_t));

	if (NULL == new_node)
	{
		return NULL;
	}

	new_node->data = data;
	new_node->prev = iter->prev;
	new_node->next = iter;

	(iter->prev)->next = new_node;
	iter->prev = new_node;

	return new_node;
}

dll_iter_t DLListRemove(dll_iter_t iter)
{
	dll_iter_t next_iter = NULL;

	assert(iter);
	
	next_iter = DLListNext(iter);
	(iter->prev)->next = next_iter;
	next_iter->prev = iter->prev;

	free(iter);
	iter = NULL;

	return next_iter;
}

dll_iter_t DLListPopFront(dl_list_t *list)
{
	assert(list);

	return DLListRemove(DLListBegin(list));
}

dll_iter_t DLListPopBack(dl_list_t *list)
{
	assert(list);

	return DLListRemove(DLListPrev(DLListEnd(list)));
}

dll_iter_t DLListFind(dll_iter_t from_iter, dll_iter_t to_iter,
		void *find_data, int (*cmp_func)(const void *data, const void *find_data))
{
	
    dll_iter_t current_node = NULL;

    assert(from_iter);
    assert(to_iter);
    assert(cmp_func);
    
    for (current_node = from_iter;
             current_node != to_iter;
                current_node = DLListNext(current_node))
    {
        int ret_value = cmp_func(DLListGetData(current_node), find_data);
        
        if (0 == ret_value)
        {
            break;
        }
    }

    return current_node;
}


int DLListForEach(dll_iter_t from_iter, dll_iter_t to_iter,
			void *param, int (*oper_func)(void *data, void *param))
{
    dll_iter_t current_node = NULL;
	int ret_value = 0;

    assert(from_iter);
    assert(to_iter);
    assert(oper_func);
    
    for (current_node = from_iter;
             current_node != to_iter;
                current_node = DLListNext(current_node))
    {
        ret_value = oper_func(DLListGetData(current_node), param);
        
        if (ret_value != 0)
        {
            break;
        }        
    }

    return ret_value;
}

void DLListSplice(dll_iter_t dest_where, dll_iter_t src_iter_from, dll_iter_t src_iter_to)
{
	dll_iter_t one_before = NULL;

	assert(dest_where);
	assert(src_iter_from);
	assert(src_iter_to);
	
	one_before = dest_where->prev;

	/* connect last node of src to dest->prev */
	(src_iter_to->prev)->next = dest_where;
	dest_where->prev = src_iter_to->prev;

	/* handle the remove of the nodes from src */
	(src_iter_from->prev)->next = src_iter_to;
	src_iter_to->prev = src_iter_from->prev;
	
	/* make dest->prev point to new list  */
	src_iter_from->prev = one_before;
	one_before->next = src_iter_from;

}

int DLListMultiFind(const dl_list_t *list, void *find_data, 
					int (*cmp_func)(const void *data, const void *find_data),
					dl_list_t *output_list)
{
	dll_iter_t iter = NULL;

	assert(list);
	assert(output_list);
	assert(cmp_func);
	
	for (iter = DLListBegin(list)
			; 0 == DLListIsSameIterator(iter, DLListEnd(list))
				; iter = DLListNext(iter))
	{
		if (0 == cmp_func(iter->data, find_data))
		{
			if (NULL == DLListPushBack(output_list, iter->data))
			{
				return 1;
			}
		}
	}
	
	return 0;
}
