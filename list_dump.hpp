#ifndef LIST_DUMP_HPP
#define LIST_DUMP_HPP

#include "list.hpp"

error_t DumpListGraph(List* list, char* filename);
error_t DumpListText(List* list, char* filename, error_t error);

#endif