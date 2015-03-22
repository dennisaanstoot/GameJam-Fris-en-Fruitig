
CC=gcc

LFLAGS= -ljson-c -lwebsockets -lm -lpthread

all: server

server: list.o entity.o game.o json.o main.o field.o
	$(CC) entity.o game.o main.o json.o list.o field.o $(LFLAGS) -o server -g

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

field.o: field.c
	$(CC) -g -c field.c -Wall

clean:
	rm *.o server
