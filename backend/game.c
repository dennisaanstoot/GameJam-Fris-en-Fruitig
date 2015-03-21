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
	struct list * curr = g->entity_list;
	while(curr != NULL)
	{
		struct entity * entity = (struct entity *) curr->e;
		entity_tick(entity, g);
		curr = curr->next;
	}
}

int game_over(struct game * g)
{
	int player_alive = 0;
	struct list * curr = g->entity_list;
	while(curr != NULL)
	{
		struct entity * entity = curr->e;
		
		if(entity->type == PLAYER)
		{
			struct player_info * info = (struct player_info*) entity->info;
			if(info->health <= 0)
			{
				player_alive++;
			}
		}
		
		curr = curr->next;
	}
	if(player_alive <= 2)
		return 1;
	return 0;
}
