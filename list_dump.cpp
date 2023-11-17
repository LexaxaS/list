#include <stdio.h>
#include <stdlib.h>
#include "tools.hpp"
#include "list.hpp"
#include "list_dump.hpp"

#define FONT_SIZE "10"
#define FONT_NAME "\"Sans Bold Not-Rotated\""
#define BACK_GROUND_COLOR "\"#5e67d4\""
#define NODE_COLOR "\"#faa116\""
#define NODE_FRAME_COLOR "\"#000000\""
#define ROOT_COLOR "\"#c25a90\""
#define FREE_COLOR "\"#b9e793\""


error_t DumpListText(List* list, char* filename, error_t error)
    {
    MY_ASSERT_HARD(list);

    FILE* outTxtFile = fileopenerW(filename);

    fprintf(outTxtFile,
        "error = %d\n"
        "length = %zu\n"
        "capacity = %zu\n"
        "free = %zu\n"
        "head = %zu\n"
        "tail = %zu\n\n"
        "list[%p]:\n"
        "   {\n",
        error, list->length, list->capacity, list->free, list->head, list->tail, list);

    size_t i = 1;
    size_t pos = list->head;
    while (pos != 0)
        {
        fprintf(outTxtFile, "   [%d] = %d\n", i, list->data[pos]);
        pos = list->next[pos];
        i++;
        }

    fprintf(outTxtFile, "   }\n\ndata[%p]\n   {\n", list->data); 

    for (size_t i = 0; i < list->capacity; i++)
        {
        fprintf(outTxtFile, "   [%d] = %d\n", i, list->data[i]);
        }

    fprintf(outTxtFile, "   }\n\nprev[%p]\n   {\n", list->prev); 

    for (size_t i = 0; i < list->capacity; i++)
        {
        fprintf(outTxtFile, "   [%d] = %d\n", i, list->prev[i]);
        }

    fprintf(outTxtFile, "   }\n\nnext[%p]\n   {\n", list->next); 

    for (size_t i = 0; i < list->capacity; i++)
        {
        fprintf(outTxtFile, "   [%d] = %d\n", i, list->next[i]);
        }
    
    fprintf(outTxtFile, "   }\n");
    fclose(outTxtFile);
    return NO_ERROR;
    }

error_t DumpListGraph(List* list, char* filename)
    {
    MY_ASSERT_HARD(list);

    FILE* outDotFile = fileopenerW(filename);

    fprintf(outDotFile, 
        "digraph\n"
        "{\n"
        "rankdir = LR;\n"
            "node[shape = record, color = " NODE_FRAME_COLOR ", fontname = " FONT_NAME ", fontsize = " FONT_SIZE "];\n"
        "bgcolor = " BACK_GROUND_COLOR ";\n"

        "ROOT[style = \"filled\", fillcolor = " ROOT_COLOR ", "
        "label = \"ROOT|{<head>head = %zu|<tail>tail = %zu}\"];\n"

        "FREE[style = \"filled\", fillcolor = " FREE_COLOR ", "
        "label = \"FREE|<free>free = %zu\"];\n",
        list->head, list->tail, list->free);
    
    for (size_t i = 1; i < list->capacity; i++)
        {
        if (list->prev[i] == -1)
            fprintf(outDotFile, 
                "CELL_%zu[style = \"filled\", fillcolor = " NODE_COLOR ", "
                "label = \"index = %zu|value\\n FREE|{prev = FREE|next = %zu}\"];\n",
                i, i, list->next[i]);
        else
            fprintf(outDotFile, 
                "CELL_%zu[style = \"filled\", fillcolor = " NODE_COLOR ", "
                "label = \"index = %zu|value\\n %d|{prev = %zu|next = %zu}\"];\n",
                i, i, list->data[i], list->prev[i], list->next[i]);
        }

    fprintf(outDotFile, "ROOT->CELL_1");
    for (size_t i = 2; i < list->capacity; i++)
        fprintf(outDotFile, "->CELL_%zu", i);

    fprintf(outDotFile, " [weight = 10000000, color = " BACK_GROUND_COLOR "];\n");
    
    if (list->head)
        fprintf(outDotFile, "ROOT:head->CELL_%zu [style = \"bold\", color = white];\n", list->head);
    
    if (list->tail)
        fprintf(outDotFile, "ROOT:tail->CELL_%zu [style = \"bold\", color = white];\n", list->tail);

    if (list->head != list->tail)
        {
        fprintf(outDotFile, "CELL_%zu", list->head);
        size_t index = list->next[list->head];
        while (index != 0)
            {
            fprintf(outDotFile, "->CELL_%zu", index);
            index = list->next[index];
            }
        fprintf(outDotFile, "[style = \"bold\", color = white];\n");
        }

    if (list->free)
        fprintf(outDotFile, "FREE:free->CELL_%zu[style = \"bold\", color = white];\n", list->free);

    fprintf(outDotFile, "}\n");

    fclose(outDotFile);
    return NO_ERROR;
    }
    