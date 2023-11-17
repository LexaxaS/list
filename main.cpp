#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "tools.hpp"
#include "list.hpp"
#include "list_dump.hpp"

int main()
    {
    List list = {};
    listCreate(&list);
    for (size_t i = 0; i < 8; i++)
        printf("%d %d\n", i, list.next[i]);

    printf("\n\n\n");

    for (size_t i = 0; i < 8; i++)
        listPushAfter(&list, i, i + 1);
    listPushAfter(&list, 6, 0);
    listPushBefore(&list, 6, 100);
    listPop(&list, 6);
    for (size_t i = 0; i < 10; i++)
        printf("%d %d\n", list.data[i], list.next[i]);
    
    printf("\n\n\n");
    DumpListGraph(&list, "listgraph1.dot");
    DumpListText(&list, "listtxt1.txt", NO_ERROR);

    straighten(&list);
    DumpListGraph(&list, "listgraph2.dot");
    DumpListText(&list, "listtxt2.txt", NO_ERROR);
    
    for (size_t i = 0; i < 10; i++)
        printf("%d ", list.data[i]);
    return 0;
    }
