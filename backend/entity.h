
#include "game.h"

enum entity_type {
	PLAYER,
	TREE,
	BULLET
};

struct entity {
	enum entity_type type;
	unsigned int x;
	unsigned int y;
	void* info;
};

struct player_info {
	unsigned int xv;
	unsigned int yv;
	unsigned int xd;
	unsigned int yd;
	int health;
};

struct bullet_info {
	unsigned int xv;
	unsigned int yv;
};

struct entity* entity_player_new(unsigned int x, unsigned int y, unsigned int xv, unsigned int yv, unsigned int xd, unsigned int yd);

struct entity* entity_tree_new(unsigned int x, unsigned int y);

struct entity* entity_bullet_new(unsigned int x, unsigned int y, unsigned int xv, unsigned int yv);

void entity_player_destroy(struct entity * e);

void entity_tree_destroy(struct entity * e);

void entity_bullet_destroy(struct entity * e);

void entity_tick(struct entity * e, struct game * game);

