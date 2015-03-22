#include "field.h"

int field_on_field(struct field field, unsigned int x, unsigned int y)
{
	if(field.width > x || 0 < x || field.height > y || 0 < y) 
	{
		return 1;
	}
	return 0;
}
