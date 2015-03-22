#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "field.h"
#include "game.h"
#include "entity.h"

struct entity* entity_player_new(unsigned int x, unsigned int y, int xv, int yv, unsigned int xd, unsigned int yd, char* name )
{
	struct entity * result = malloc(sizeof(struct entity));
	result->type = PLAYER;
	result->x = x;
	result->y = y;
	struct player_info * info = malloc(sizeof(struct player_info));
	info->xv = xv;
	info->yv = yv;
	info->xd = xd;
	info->yd = yd;
	info->health = 100;
	char* string = malloc(30);
	strncpy(string, name, 29);
	info->name = string;
	result->info = info;
	return result;
}

struct entity* entity_bullet_new(unsigned int x, unsigned int y, int xv, int yv)
{
	struct entity * result = malloc(sizeof(struct entity));
	result->type = BULLET;
	result->x = x;
	result->y = y;
	struct bullet_info * info = malloc(sizeof(struct bullet_info));
	info->xv = xv;
	info->yv = yv;
	info->ticks = 0;
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
	struct player_info * info = (struct player_info *) e->info;
	free(info->name);
	free(info);
	free(e);
}

void entity_bullet_destroy(struct entity * e)
{
	struct bullet_info * info = (struct bullet_info *) e->info;
	free(info);
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

	double a = atan2((int) e->y - (int) info->yd, (int) e->x - (int) info->xd);

	double xv = cos(a) * 2.0;
	double yv = sin(a) * 2.0;

	info->xv -= (int) xv;
	info->yv -= (int) yv;

	e->x += info->xv;
	e->y += info->yv;
}

void bullet_tick(struct entity * e, struct game * game)
{
	struct bullet_info * info = (struct bullet_info*) e->info;
	
	e->x += info->xv;
	e->y += info->yv;
}

struct entity * entity_player_shoot(struct entity * e, struct game * g, unsigned int x, unsigned int y)
{
	struct list * e_list = g->entity_list;
	double a = atan2(e->y - y, e->x - x);
	
	int xv = 4.0 * cos(a);
	int yv = 4.0 * sin(a);

	struct entity * bullet = entity_bullet_new(e->x, e->y, xv, yv);

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

