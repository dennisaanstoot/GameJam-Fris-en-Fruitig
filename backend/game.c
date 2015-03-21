#include <stdlib.h>

#include "field.h"
#include "list.h"
#include "game.h"
#include "entity.h"

struct game * game_new(struct field f, struct list * entity_list)
{
	struct game * result = malloc(sizeof(struct game));
	result->field = f;
	result->entity_list = entity_list;
	return result;
}

void game_destroy(struct game* game)
{
	free(game);
}

void game_tick(struct game * g)
{
	int i;
	struct list * l = g->entity_list;
	for(i = 0; i < l->length; i++) 
	{
		struct entity * entity = (struct entity *) l->array[i];
		entity_tick(entity, g);
	}
}

int game_over(struct game * g)
{
	return 0;
	int player_alive = 0;
	int i;
	struct list * l = g->entity_list;
	for(i = 0; i < l->length; i++) 
	{
		struct entity * entity = (struct entity *) l->array[i];
		
		if(entity->type == PLAYER)
		{
			struct player_info * info = (struct player_info*) entity->info;
			if(info->health <= 0)
			{
				player_alive++;
			}
		}
		
	}
	if(player_alive <= 2)
		return 1;
	return 0;
}
