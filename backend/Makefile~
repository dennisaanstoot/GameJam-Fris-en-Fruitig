
CC=clang

LFLAGS= -lwebsockets -lm

all: server

server: main.o entity.o entity_list.o
	$(CC) main.o entity.o $(LFLAGS) -o server -g

main.o: main.c
	$(CC) -g -c main.c

entity.o: entity.c
	$(CC) -g -c entity.c

entity_list.o: entity_list.c
	$(CC) -g -c entity_list.c


clean:
	rm *.o server
