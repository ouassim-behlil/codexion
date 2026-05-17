#include "include/codexion.h"

static void swap(t_request *req1, t_request *req2)
{
    t_request       temp_req;

    if (req1 == req2)
    {
        return ;
    }
    temp_req = *req1;
    *req1 = *req2;
    *req2 = temp_req;
}

void heap_insert(t_heap *heap, t_request req)
{
	int			idx;
	int			parent_idx;
	t_request	*requests;

    requests = heap->requests;
	// place the req at the end of the heap array
	requests[heap->size] = req;

	// compare element with parent and swap if necessary
	idx = heap->size;
	parent_idx = (idx - 1) / 2;
	while (idx > 0 && requests[idx].key < requests[parent_idx].key)
	{
		swap(&requests[idx], &requests[parent_idx]);
		idx = parent_idx;
		parent_idx = (idx - 1) / 2;
	}
	heap->size ++;
}

void    heap_delete(t_heap *heap)
{
    int         idx;
    int         left_child_idx;
    int         right_child_idx;
    int         smallest;
    t_request   *requests;

    if (heap->size == 0)
        return ;
    requests = heap->requests;
    // replace the root with last element and delete it
    requests[0] = requests[heap->size - 1];
    heap->size --;

    // heapify down to recover minimum at top
    idx = 0;
    while (idx < heap->size)
    {
        smallest = idx;
        left_child_idx = idx * 2 + 1;
        right_child_idx = idx * 2 + 2;
    
        if (left_child_idx < heap->size && requests[smallest].key > requests[left_child_idx].key)
        {
            smallest = left_child_idx;
        }
        if (right_child_idx < heap->size && requests[smallest].key > requests[right_child_idx].key)
        {
            smallest = right_child_idx;
        }
        if (smallest != idx)
        {
            swap(&requests[smallest], &requests[idx]);
            idx = smallest;
        }
        else
        {
            return ;
        }
    }
}

// heap_peek returns the coder id at the root of the heap
int     heap_peek(t_heap *heap)
{
    if (heap->size == 0)
        return (-1);
    return (heap->requests[0].coder_id);
}