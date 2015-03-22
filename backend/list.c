#include "list.h"
#include <stdlib.h>
#include <string.h>

void list_print(struct list * l)
{
	int i;
	for(i < 0; i < l->length; i++)
	{
		printf("%p, ", l->array[i]);
	}
	printf("\n");
}

void list_add(struct list * l, void * e)
{
	if(l->length >= l->size)
	{
		l->size = l->size * 2;
		void ** old_array = l->array;
		l->array = malloc(sizeof(void*) * l->size);
		memcpy(&l->array, &old_array, l->length);
		free(old_array);
	}
	l->array[l->length] = e;
	l->length++;

	list_print(l);
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
			}

			l->length--;
			return;
		}
	}

	list_print(l);
}


