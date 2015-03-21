#include "list.h"
#include <stdlib.h>
#include <string.h>

void list_add(struct list * l, void * e)
{

	if(l->length >= l->size)
	{
		unsigned int size = l->size * 2;
		free(l->array);
		l->array = malloc(sizeof(void*) * size);
	}
	l->array[++l->length] = e;

	/*
        struct list * curr = list;
        while(curr->next != NULL) 
        {
                curr = curr->next;
        }
        struct list * new_list = malloc(sizeof(struct list));
        new_list->e = e;
        new_list->next = 0;
        curr->next = new_list;
	*/
}

void list_remove(struct list * l, void * e)
{

	int i;
	for(i = 0; i < l->size; i++)
	{
		if(l->array[i] == e)
		{
			memcpy(&l->array[i], &l->array[i+1], l->length-1);
			return;
		}
	}

	/*
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

	*/
}

