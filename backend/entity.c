#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "entity.h"


struct entity* entity_player_new(unsigned int x, unsigned int y, unsigned int xv, unsigned int yv, unsigned int xd, unsigned int yd) 
{
	struct entity * result = malloc(sizeof(struct entity));
	result->type = PLAYER;
	result->x = x;
	result->y = y;
	struct player_info * info = malloc(sizeof(struct player_info));
	info->xv = 0;
	info->yv = 0;
	info->xd = 0;
	info->yd = 0;
	info->health = 0;
	result->info = info;
	return result;
}

struct entity* entity_tree_new(unsigned int x, unsigned int y) 
{
	struct entity * result = malloc(sizeof(struct entity));
	result->x = x;
	result->y = y;
	result->type = TREE;
	return result;
}

void entity_player_destroy(struct entity * e) 
{
	free(e->info);
	free(e);
}

void entity_tree_destroy(struct entity * e) 
{
	free(e);
}

void player_tick(struct entity * e, struct game * game)
{
	struct player_info * info = (struct player_info*) e->info;

	if(info->health <= 0)
	{
		return;
	}

	double a = atan2(info->xd - e->x, info->yd - e->y);

	info->xv += (unsigned int) (5.0 * cos(a));
	info->yv += (unsigned int) (5.0 * sin(a));

	e->x += info->xv;
	e->y += info->yv;
}

void bullet_tick(struct entity * e, struct game * game)
{
	struct bullet_info * info = (struct bullet_info*) e->info;

	e->x += info->xv;
	e->y += info->yv;
}

void entity_tick(struct entity * e, struct game * game)
{
	switch(e->type) {
	case PLAYER:
		player_tick(e, game);
		break;
	case BULLET:
		bullet_tick(e, game);
	default:
		break;
		
	}
}

