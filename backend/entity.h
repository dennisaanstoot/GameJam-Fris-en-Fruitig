
enum entity_type {
	PLAYER,
	TREE,
	BULLET,
	HEALTH_CRATE
};

struct entity {
	enum entity_type type;
	double x;
	double y;
	void* info;
};

struct player_info {
	double xv;
	double yv;
	unsigned int xd;
	unsigned int yd;
	double prev_angle;
	double angle_vel;
	double angle;
	int thrust;
	int health;
	char* name;
	unsigned int timeout;
};

struct bullet_info {
	double xv;
	double yv;
	unsigned int ticks;
};

struct entity* entity_player_new(double x, double y, double xv, double yv, unsigned int xd, unsigned int yd, char* name);

struct entity* entity_tree_new(double x, double y);

struct entity* entity_bullet_new(double x, double y, double xv, double yv);

struct entity* entity_health_new(double x, double y);

void entity_player_destroy(struct entity * e);

void entity_tree_destroy(struct entity * e);

void entity_bullet_destroy(struct entity * e);

void entity_health_destroy(struct entity * e);

void entity_player_shoot(struct entity *e, struct game *g, unsigned int x, unsigned int y);

void entity_tick(struct entity * e, struct game * game);

