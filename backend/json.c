#include <json-c/json.h>
#include <stdio.h>
#include <math.h>

#include "field.h"
#include "list.h"
#include "game.h"
#include "entity.h"

json_object* json_frame(struct game* g)
{
	struct player_info * p_info;
	struct bullet_info * b_info;

	json_object * jarray = json_object_new_array();
	json_object * jobj = NULL;

	json_object * jtype = NULL;
	json_object * jx = NULL;
	json_object * jy = NULL;
	json_object * jangle = NULL;
	json_object * jhealth = NULL;
	json_object * jname = NULL;

	int i;
	struct entity * e;
	struct list * l = g->entity_list;

	for(i = 0; i < l->length; i++)
	{
		e = (struct entity *) l->array[i];
		jobj = json_object_new_object();
		switch(e->type) {
		case PLAYER:
			p_info = (struct player_info *) e->info;
			
			jtype = json_object_new_string("player");
			jx = json_object_new_int(e->x);
			jy = json_object_new_int(e->y);
			jangle = json_object_new_double(p_info->angle);
			jhealth = json_object_new_int(p_info->health);
			jname = json_object_new_string(p_info->name);

			json_object_object_add(jobj, "entity_type", jtype); 
			json_object_object_add(jobj, "x", jx);
			json_object_object_add(jobj, "y", jy);
			json_object_object_add(jobj, "angle", jangle);
			json_object_object_add(jobj, "health", jhealth);
			json_object_object_add(jobj, "name", jname);

			break;
		case BULLET:
			b_info = (struct bullet_info *) e->info;

			jtype = json_object_new_string("bullet");
			jx = json_object_new_int(e->x);
			jy = json_object_new_int(e->y);
			jangle = json_object_new_double(M_PI + atan2(b_info->yv, b_info->xv));

			json_object_object_add(jobj, "entity_type", jtype); 
			json_object_object_add(jobj, "x", jx);
			json_object_object_add(jobj, "y", jy);
			json_object_object_add(jobj, "angle", jangle);

			break;
		case TREE:
			jtype = json_object_new_string("tree");
			jx = json_object_new_int(e->x);
			jy = json_object_new_int(e->y);
			
			json_object_object_add(jobj, "entity_type", jtype); 
			json_object_object_add(jobj, "x", jx);
			json_object_object_add(jobj, "y", jy);

			break;
		case HEALTH_CRATE:
			jtype = json_object_new_string("health_crate");
			jx = json_object_new_int(e->x);
			jy = json_object_new_int(e->y);
			
			json_object_object_add(jobj, "entity_type", jtype); 
			json_object_object_add(jobj, "x", jx);
			json_object_object_add(jobj, "y", jy);

			break;
		}
		json_object_array_add(jarray, jobj);
	}
	return jarray;
}

