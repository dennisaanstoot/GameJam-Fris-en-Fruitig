
CC=gcc

LFLAGS= -ljson-c -lwebsockets -lm -lpthread

all: server

server: list.o entity.o game.o json.o main.o
	$(CC) entity.o game.o main.o json.o list.o $(LFLAGS) -o server -g

main.o: main.c
	$(CC) -g -c main.c -Wall

entity.o: entity.c
	$(CC) -g -c entity.c -Wall

list.o: list.c
	$(CC) -g -c list.c -Wall

json.o: json.c
	$(CC) -g -c json.c -Wall

game.o: game.c
	$(CC) -g -c game.c -Wall

clean:
	rm *.o server
