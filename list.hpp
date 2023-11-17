#ifndef LIST_HPP
#define LIST_HPP

#include <stdio.h>
#include "tools.hpp"

typedef int elem_t;
#define elem_pr "%d"

const elem_t POISON_LIST = 123456789;
const size_t FREE_CELL = -1;
const size_t MIN_LIST_CAPACITY = 8;

struct List
    {
    elem_t* data;
    size_t* next;
    size_t* prev;

    size_t free;
    size_t head;
    size_t tail;

    size_t length;
    size_t capacity;
    };


enum Errors
    {NO_ERROR = 0,
     ALLOC_ERROR = 1,
     POINTER_ERROR = 2,
     WRONG_CAP_ERROR = 3};

error_t listCreate(List* list);
error_t listDestruct(List* list);

error_t listPushAfter(List* list, size_t position, elem_t value);
error_t listPushBefore(List* list, size_t position, elem_t value);
elem_t listPop(List* list, size_t position);

error_t listVerify(List* list);
error_t _listRealloc(List* list);
error_t _listReallocUp(List* list, size_t newCapacity);
error_t straighten(List* list);

#endif