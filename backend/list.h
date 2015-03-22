struct list {
	void ** array;
	unsigned int size;
	unsigned int length;
};

void list_add(struct list * list, void * e);
void list_remove(struct list * list, void * e);

