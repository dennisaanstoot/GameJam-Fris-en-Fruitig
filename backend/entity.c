#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "list.h"
#include "field.h"
#include "game.h"
#include "entity.h"


int distance(struct entity * e1, struct entity * e2)
{
	return sqrt(pow(e1->x - e2->x,2) + pow(e1->y - e2->y,2));
}

struct entity* entity_player_new(unsigned int x, unsigned int y, double xv, double yv, unsigned int xd, unsigned int yd, char* name )
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

struct entity* entity_bullet_new(unsigned int x, unsigned int y, double xv, double yv)
{
	struct entity * result = malloc(sizeof(struct entity));
	result->type = BULLET;
	result->x = x;
	result->y = y;
	struct bullet_info * info = malloc(sizeof(struct bullet_info));
	info->xv = xv;
	info->yv = yv;
	info->ticks = 0;
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


	if(!field_on_field(game->field, e->x, e->y) )
	{
		info->health -= 10;
	}

	if(info->health <= 0)
	{
		list_remove(game->entity_list, e);
		entity_player_destroy(e);
	}

	double a = atan2((int) e->y - (int) info->yd, (int) e->x - (int) info->xd);

	double xv = cos(a) * 2.0;
	double yv = sin(a) * 2.0;

	info->xv -= xv;
	info->yv -= yv;

	if(info->xv > 3.0)
		info->xv = 3.0;
        if(info->yv > 3.0)
		info->yv = 3.0;	

	e->x += (int) info->xv;
	e->y += (int) info->yv;
}

void bullet_tick(struct entity * e, struct game * g)
{
	struct bullet_info * info = (struct bullet_info*) e->info;
	struct player_info * p_info;
	struct list * e_list = g->entity_list;
	struct entity * e2;

	int i;

	e->x += (int) info->xv;
	e->y += (int) info->yv;

	info->ticks++;

	if(!field_on_field(g->field, e->x, e->y) )
	{
		list_remove(e_list, e);
		entity_bullet_destroy(e);
	}
	else if(info->ticks > 30) 
	{ 
		for(i = 0; i < e_list->length; i++)
		{
			e2 = e_list->array[i];
			if(e2->type == PLAYER && distance(e,e2) < 4) {
				p_info = (struct player_info*) e2->info;
				p_info->health -= 10;
				list_remove(e_list, e);
				entity_bullet_destroy(e);
			}
		}
	}
}

void tree_tick(struct entity * e, struct game * g)
{
	struct player_info * p_info;
	struct list * e_list = g->entity_list;
	struct entity * e2;
	int i;
	for(i = 0; i < e_list->length; i++)
	{
		e2 = e_list->array[i];
		if(distance(e,e2) < 4) {
			switch(e2->type) {
			case(BULLET):
				list_remove(e_list, e2);
				entity_bullet_destroy(e2);
				break;
			case(PLAYER):
				p_info = (struct player_info *) e2->info;
				if(p_info->xv > 1.0)
					p_info->xv = 1.0;
				if(p_info->yv > 1.0)
					p_info->yv = 1.0;
				break;
			}	
		}
	}
}

void entity_player_shoot(struct entity * e, struct game * g, unsigned int x, unsigned int y)
{
	struct list * e_list = g->entity_list;
	double a = atan2(e->y - y, e->x - x);
	
	int xv = 3.0 * cos(a);
	int yv = 3.0 * sin(a);

	struct entity * bullet = entity_bullet_new(e->x, e->y, xv, yv);

	list_add(e_list, bullet);
}

void entity_tick(struct entity * e, struct game * game)
{
	switch(e->type) {
	case PLAYER:
		player_tick(e, game);
		break;
	case BULLET:
		bullet_tick(e, game);
		break;
	case TREE:
		tree_tick(e, game);
		break;
	default:
		break;
		
	}
}

