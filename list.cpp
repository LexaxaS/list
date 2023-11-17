#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "list.hpp"



error_t listCreate(List* list)
    {
    MY_ASSERT_HARD(list);

    elem_t* data = (elem_t*) calloc(MIN_LIST_CAPACITY, sizeof(*data));
    size_t* next = (size_t*) calloc(MIN_LIST_CAPACITY, sizeof(*next));
    size_t* prev = (size_t*) calloc(MIN_LIST_CAPACITY, sizeof(*prev));

    MY_ASSERT_SOFT(data, ALLOC_ERROR);
    MY_ASSERT_SOFT(next, ALLOC_ERROR);
    MY_ASSERT_SOFT(prev, ALLOC_ERROR);

    for (size_t i = 0; i < MIN_LIST_CAPACITY; i++)
        {
        data[i] = POISON_LIST;
        next[i] = i + 1;
        prev[i] = FREE_CELL;
        }
    next[0] = 0;
    prev[0] = 0;
    list->data = data;
    list->next = next;
    list->prev = prev;

    list->head = 1;
    list->capacity = MIN_LIST_CAPACITY;
    list->length = 0;
    list->free = 1;
    list->tail = 1;

    return NO_ERROR;
    }

error_t listDestruct(List* list)
    {
    MY_ASSERT_SOFT(list, POINTER_ERROR);
    MY_ASSERT_SOFT(list->data, POINTER_ERROR);
    MY_ASSERT_SOFT(list->next, POINTER_ERROR);
    MY_ASSERT_SOFT(list->prev, POINTER_ERROR);
    
    free(list->data);
    free(list->next);
    free(list->prev);
    free(list);

    return NO_ERROR;
    }
error_t listPushAfter(List* list, size_t position, elem_t value)
    {
    MY_ASSERT_SOFT(list, POINTER_ERROR);
    MY_ASSERT_SOFT(list->data, POINTER_ERROR);
    MY_ASSERT_SOFT(list->next, POINTER_ERROR);
    MY_ASSERT_SOFT(list->prev, POINTER_ERROR);

    size_t pushInd = list->free;
    list->free = list->next[list->free];
    list->length++;

    list->data[pushInd] = value;
    list->prev[pushInd] = position;
    list->next[pushInd] = list->next[position];

    list->prev[list->next[position]] = pushInd;
    list->next[position] = pushInd;
    if (list->tail == position)
        list->tail = pushInd;

    printf("\npind = %d pos = %d next = %d tail = %d\n", pushInd, position, list->next[position], list->tail);
    if (list->head - 1 == position)
        list->head = pushInd;
    // if (list->tail == pushInd)
    //     list->next[pushInd] = 0;
    return _listRealloc(list);
    }

error_t listPushBefore(List* list, size_t position, elem_t value)
    {
    MY_ASSERT_SOFT(list, POINTER_ERROR);
    MY_ASSERT_SOFT(list->data, POINTER_ERROR);
    MY_ASSERT_SOFT(list->next, POINTER_ERROR);
    MY_ASSERT_SOFT(list->prev, POINTER_ERROR);

    size_t pushInd = list->free;
    list->free = list->next[list->free];
    list->length++;

    list->data[pushInd] = value;
    list->next[pushInd] = position;
    list->prev[pushInd] = list->prev[position];

    list->next[list->prev[pushInd]] = pushInd;
    list->prev[position] = pushInd;

    if (list->tail + 1 == position)
        list->tail = pushInd;

    if (list->head == position)
        list->head = pushInd;

    return _listRealloc(list);
    }

elem_t listPop(List* list, size_t position)
    {
    MY_ASSERT_SOFT(list, POINTER_ERROR);
    MY_ASSERT_SOFT(list->data, POINTER_ERROR);
    MY_ASSERT_SOFT(list->next, POINTER_ERROR);
    MY_ASSERT_SOFT(list->prev, POINTER_ERROR);

    elem_t value = list->data[position];

    list->data[position] = POISON_LIST;
    list->length--;

    list->next[list->prev[position]] = list->next[position];
    list->prev[list->next[position]] = list->prev[position];

    list->prev[position] = FREE_CELL;
    list->next[position] = list->free;
    list->free = position;

    return value;
    }

error_t _listRealloc(List* list)
    {
    MY_ASSERT_HARD(list);
    
    if (list->length + 1 == list->capacity)
        return _listReallocUp(list, list->capacity * 2);

    return NO_ERROR;
    }


error_t _listReallocUp(List* list, size_t newCapacity)
    {
    MY_ASSERT_SOFT(list, POINTER_ERROR);
    MY_ASSERT_SOFT(list->data, POINTER_ERROR);
    MY_ASSERT_SOFT(list->next, POINTER_ERROR);
    MY_ASSERT_SOFT(list->prev, POINTER_ERROR);


    elem_t* newData = (elem_t*) realloc(list->data, newCapacity * sizeof(*newData));
    size_t* newNext = (size_t*) realloc(list->next, newCapacity * sizeof(*newNext));
    size_t* newPrev = (size_t*) realloc(list->prev, newCapacity * sizeof(*newPrev));


    MY_ASSERT_SOFT(newData, ALLOC_ERROR);
    MY_ASSERT_SOFT(newNext, ALLOC_ERROR);
    MY_ASSERT_SOFT(newPrev, ALLOC_ERROR);


    for (size_t i = list->capacity; i < newCapacity; i++)
        {
        newData[i] = POISON_LIST;
        newNext[i] = i + 1;
        newPrev[i] = FREE_CELL;
        }
    newNext[newCapacity - 1] = 0;

    list->data = newData;
    list->next = newNext;
    list->prev = newPrev;

    list->free = list->capacity;
    list->capacity = newCapacity;

    return NO_ERROR;
    }

error_t listVerify(List* list)
    {
    MY_ASSERT_SOFT(list, POINTER_ERROR);

    if (!list->data || !list->prev || !list->next)
        return POINTER_ERROR;
    if (list->capacity < list->length)
        return WRONG_CAP_ERROR;
    return NO_ERROR;
    }

error_t straighten(List* list)
    {
    MY_ASSERT_SOFT(list, POINTER_ERROR);
    MY_ASSERT_SOFT(list->data, POINTER_ERROR);
    MY_ASSERT_SOFT(list->next, POINTER_ERROR);
    MY_ASSERT_SOFT(list->prev, POINTER_ERROR);

    size_t newCapacity = max(MIN_LIST_CAPACITY, list->length + 1) + 1;

    elem_t* newData = (elem_t*) calloc(newCapacity, sizeof(*newData));
    size_t* newNext = (size_t*) calloc(newCapacity, sizeof(*newNext));
    size_t* newPrev = (size_t*) calloc(newCapacity, sizeof(*newPrev));


    MY_ASSERT_SOFT(newData, ALLOC_ERROR);
    MY_ASSERT_SOFT(newNext, ALLOC_ERROR);
    MY_ASSERT_SOFT(newPrev, ALLOC_ERROR);


    for (size_t i = 0; i < newCapacity; i++)
        {
        newData[i] = POISON_LIST;
        newNext[i] = i + 1;
        newPrev[i] = FREE_CELL;
        }
    newNext[newCapacity - 1] = 0;

    size_t ind = 1;
    size_t pos = list->head;
    while (pos != 0)
        {
        printf("pos = %d, ind = %d, data = %d, next = %d\n", pos, ind, list->data[pos], list->next[pos]);
        newData[ind] = list->data[pos];
        newPrev[ind] = ind - 1;
        pos = list->next[pos];
        ind++;
        }

    newNext[ind--] = 0;
    newNext[0] = 1;
    newPrev[0] = ind;

    for (size_t i = 0; i < newCapacity; i++)
        {
        printf("%d %d %d\n", newData[i], newNext[i], newPrev[i]);
        }
    free(list->data);
    free(list->next);
    free(list->prev);

    list->data = newData;
    list->next = newNext;
    list->prev = newPrev;

    list->head = 1;
    list->tail = ind;
    list->free = list->length + 1;
    list->capacity = newCapacity;


    return NO_ERROR;
    }