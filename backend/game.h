// #include "field.h"
// #include "list.h"

struct game {
	struct field field;
	struct list * entity_list; 
};

struct game * game_new(struct field f, struct list * entity_list);
void game_destroy(struct game* game);
void game_tick(struct game * g);
int game_over(struct game * g);
struct entity * game_get_winner(struct game * g);
