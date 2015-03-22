
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
	int xv;
	int yv;
	unsigned int xd;
	unsigned int yd;
	int health;
	char* name;
};

struct bullet_info {
	int xv;
	int yv;
	unsigned int ticks;
};

struct entity* entity_player_new(unsigned int x, unsigned int y, int xv, int yv, unsigned int xd, unsigned int yd, char* name);

struct entity* entity_tree_new(unsigned int x, unsigned int y);

struct entity* entity_bullet_new(unsigned int x, unsigned int y, int xv, int yv);

void entity_player_destroy(struct entity * e);

void entity_tree_destroy(struct entity * e);

void entity_bullet_destroy(struct entity * e);

void entity_tick(struct entity * e, struct game * game);

