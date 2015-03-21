#include "list.h"
#include <stdlib.h>

void list_add(struct list * list, void * e)
{
        struct list * curr = list;
        while(curr->next != NULL) 
        {
                curr = curr->next;
        }
        struct list * new_list = malloc(sizeof(struct list));
        new_list->e = e;
        new_list->next = 0;
        curr->next = new_list;
}

struct list * list_remove(struct list * list, void * e)
{
        struct list * prev = NULL;
        struct list * curr = list;
        int done = 0;
        while(!done)
        {
                if(e == curr->e)
                {
                        prev->next = curr->next;
                        free(curr);
                        if(curr == list)
                        {
                                list = list->next;
                        }
                        done = 1;
                }
                else if(curr->next == NULL)
                {
                        done = 1;
                }
                else
                {
                        prev = curr;
                        curr = curr->next;
                }
        }
        return list;
}
