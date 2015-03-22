#include "list.h"
#include <stdlib.h>
#include <string.h>

void list_print(struct list * l)
{
	int i;
	for(i = 0; i < l->length; i++)
	{
		printf("%p, ", l->array[i]);
	}
	printf("\n");
}

void list_add(struct list * l, void * e)
{
	int i;
	if(l->length >= l->size)
	{
		l->size = l->size * 2;
		void ** old_array = l->array;
		l->array = (void**) malloc(sizeof(void*) * l->size);
		for(i = 0; i < l->length; i++)
		{
			l->array[i] = old_array[i]; 
		}
		free(old_array);
	}
	l->array[l->length] = e;
	l->length++;
	printf("Size: %d \tLength: %d\n", l->size, l->length);
}

void list_remove(struct list * l, void * e)
{
	int i;
	for(i = 0; i < l->length; i++)
	{
		if(l->array[i] == e)
		{
			while(i < l->length -1)
			{
				l->array[i] = l->array[i+1];
				i++;
			}

			l->length--;
			return;
		}
	}
}


