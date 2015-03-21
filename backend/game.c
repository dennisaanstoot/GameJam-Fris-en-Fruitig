
#include "field.h"
#include "entity_list.h"

struct game * game_new(struct field f, struct entity_list entity_list)
{
	struct game * result = malloc(size(struct game));
	game->field = f;
	game->entity_list = entity_list;
	return result;
}

void game_destroy(struct game* game)
{
	free(game);
}

void game_tick(struct game * g)
{
	struct entity_list * curr = g->entity_list;
	while(curr != NULL)
	{
		struct entity entity = curr->entity;
		entity_tick(entity, g);
		curr = curr->next;
	}
}

int game_over(struct game * g)
{
	int player_alive = 0;
	struct entity_list * curr = g->entity_list;
	while(curr != NULL)
	{
		struct entity entity = curr->entity;
		
		if(entity->entity_type == PLAYER)
		{
			struct player_info * info = (struct player_info*) entity->info;
			if(info->health <= 0)
			{
				player_alive++;
			}
		}
		
		curr = curr->next;
	}	
}
