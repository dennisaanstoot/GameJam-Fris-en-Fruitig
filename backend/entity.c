#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "list.h"
#include "field.h"
#include "game.h"
#include "entity.h"

void error() {
	printf("Error\n");
	exit(-1);
}

int distance(struct entity * e1, struct entity * e2)
{
	return (int) sqrt(pow(e1->x - e2->x,2) + pow( e1->y - e2->y,2));
}

struct entity* entity_player_new(double x, double y, double xv, double yv, unsigned int xd, unsigned int yd, char* name )
{
	struct entity * result = malloc(sizeof(struct entity));
	if(!result) {
		error();
	}
	result->type = PLAYER;
	result->x = x;
	result->y = y;
	struct player_info * info = malloc(sizeof(struct player_info));
	if(!info) {
		error();
	}
	info->xv = xv;
	info->yv = yv;
	info->xd = xd;
	info->yd = yd;
	info->health = 1000;
	info->timeout = 0;
	char* string = malloc(30);
	strncpy(string, name, 29);
	info->name = string;
	result->info = info;
	return result;
}

struct entity* entity_bullet_new(double x, double y, double xv, double yv)
{
	struct entity * result = malloc(sizeof(struct entity));
	if(!result) {
		error();
	}
	result->type = BULLET;
	result->x = x;
	result->y = y;
	struct bullet_info * info = malloc(sizeof(struct bullet_info));
	if(!info) {
		error();
	}
	info->xv = xv;
	info->yv = yv;
	info->ticks = 0;
	result->info = info;
	return result;
}

struct entity* entity_tree_new(double x, double y) 
{
	struct entity * result = malloc(sizeof(struct entity));
	if(!result) {
		error();
	}
	result->x = x;
	result->y = y;
	result->type = TREE;
	return result;
}

struct entity* entity_health_new(double x, double y) 
{
	struct entity * result = malloc(sizeof(struct entity));
	if(!result) {
		error();
	}
	result->x = x;
	result->y = y;
	result->type = HEALTH_CRATE;;
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

void entity_health_destroy(struct entity * e) 
{
	free(e);
}

void move_to_dest(struct entity * e, struct game * game)
{
    	struct player_info * info = (struct player_info*) e->info;

    	info->prev_angle = atan2((int) e->y - (int) info->yd, (int) e->x - (int) info->xd);

    	if(info->prev_angle < 0)
    	{
        	info->prev_angle += 2 * M_PI;
    	}
	
	if( !( (info->angle - info->prev_angle <= info->angle_vel && info->angle - info->prev_angle >= 0) && (info->prev_angle - info->angle <= - info->angle_vel && info->angle_vel - info->angle >= 0 ) ) )
	{
		if( (info->angle < info->prev_angle && info->prev_angle - info->angle < M_PI) || 
			( info->angle - info->prev_angle >= M_PI && info->angle > M_PI ) ) 
		{
		  	info->angle_vel = 0.3;
		}
        	else
		{
		  	info->angle_vel = -0.3;
		}
	}

}

void player_tick(struct entity * e, struct game * game)
{
	struct player_info * info = (struct player_info*) e->info;
	struct entity * e2;
	int i;
	double xv = 0;
	double yv = 0;

	if(!field_on_field(game->field, (unsigned int) e->x, (unsigned int) e->y) )
	{
		info->health -= 2;
	}

	if(info->health <= 0)
	{
		list_remove(game->entity_list, e);
		entity_player_destroy(e);
	}

	if(info->thrust)
	{
		xv = cos(info->angle) * 0.09;
		yv = sin(info->angle) * 0.09;

		info->xv -= xv;
		info->yv -= yv;
	}

	for(i = 0; i < game->entity_list->length; i++)
	{
		e2 = game->entity_list->array[i];
		if(e2->type == TREE && distance(e,e2) < 20) {
			info->xv *= 0.9;
			info->yv *= 0.9;
		}
	}

	info->xv *= 0.96;
	info->yv *= 0.96;

	e->x += info->xv;
	e->y += info->yv;

	if( (info->angle - info->prev_angle <= info->angle_vel && info->angle - info->prev_angle >= 0) && (info->prev_angle - info->angle <= - info->angle_vel && info->angle_vel - info->angle >= 0 ) )
	{
		info->angle = info->prev_angle;
		info->angle_vel = 0;
	}
	if(info->thrust) {
		info->angle += info->angle_vel;
	}
	if(info->angle < 0)
		info->angle += 2*M_PI;
	if(info->angle > 2*M_PI)
		info->angle -= 2*M_PI;
	//if( (e->x - info->xd <= RADIUS && e->x - info->xd >= 0) || ( info->xd - e->x <= RADIUS && info->xd - e->x >= 0 ) || ( e->y - info->yd <= RADIUS && e->y - info->yd >=0) || (info->yd - e->y <= RADIUS && info->yd - e->y >=0) ) 
	
	struct entity e3 = {BULLET, info->xd, info->yd};
	if(distance(e,&e3) < 10)
	{
		info->thrust = 0;
		info->angle_vel = 0;
	}	
	else
	{
		move_to_dest(e,game);	
	}

	info->timeout++;
}

void bullet_tick(struct entity * e, struct game * g)
{
	struct bullet_info * info = (struct bullet_info*) e->info;
	struct player_info * p_info;
	struct list * e_list = g->entity_list;
	struct entity * e2;

	int i;

	e->x += info->xv;
	e->y += info->yv;

	info->ticks++;

	if(!field_on_field(g->field, (unsigned int) e->x, (unsigned int) e->y) )
	{
		list_remove(e_list, e);
		entity_bullet_destroy(e);
	}
	else if(info->ticks > 15) 
	{ 
		for(i = 0; i < e_list->length; i++)
		{
			e2 = e_list->array[i];
			if(e2->type == PLAYER && distance(e,e2) < 40) {
				p_info = (struct player_info*) e2->info;
				p_info->health -= 100;

				p_info->xv += info->xv * 0.75;
				p_info->yv += info->yv * 0.75;

				list_remove(e_list, e);
				entity_bullet_destroy(e);
				break;
			}
			else if(e2->type == TREE && distance(e,e2) < 20) {
				list_remove(e_list, e);
				entity_bullet_destroy(e);
				break;
			}
		}
	}
}

void tree_tick(struct entity * e, struct game * g)
{
}

void health_tick(struct entity * e, struct game * game)
{
	int i;
	struct list * e_list = game->entity_list;
	struct entity * e2;
	struct player_info * info;
	for(i = 0; i < e_list->length; i++)
	{
		e2 = e_list->array[i];
		info = e2->info;
		if(e2->type == PLAYER && distance(e,e2) < 40) {
			info->health += 500;
			list_remove(e_list, e);
			entity_health_destroy(e);
		}
	}

}

void entity_player_shoot(struct entity * e, struct game * g, unsigned int x, unsigned int y)
{
	struct player_info * p_info = e->info;
	if(p_info->timeout > 100) {
		struct list * e_list = g->entity_list;

		double a = atan2( y - e->y, x - e->x);
		
		double xv = 5.0 * cos(a);
		double yv = 5.0 * sin(a);
	
		struct entity * bullet = entity_bullet_new(e->x, e->y, xv, yv);
	
		list_add(e_list, bullet);

		p_info->timeout = 0;
	}
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
	case HEALTH_CRATE:
		health_tick(e, game);
	default:
		break;
		
	}
}

