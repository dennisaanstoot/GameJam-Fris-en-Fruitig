struct list {
	void* e;
	struct list* next;
};

void list_add(struct list * list, void * e);
struct list * list_remove(struct list * list, void * e);

