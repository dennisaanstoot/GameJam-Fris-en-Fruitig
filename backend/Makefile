
CC=gcc

LFLAGS = -L/usr/local/lib -lssl -lcrypto -lz -ljson-c -lwebsockets -lm -lpthread
CFLAGS = -Wall -O2 -I/usr/local/include

all: server

server: list.o entity.o game.o json.o main.o field.o
	$(CC) entity.o game.o main.o json.o list.o field.o $(LFLAGS) -o server -g

main.o: main.c
	$(CC) $(CFLAGS) -c main.c 

entity.o: entity.c
	$(CC) $(CFLAGS) -c entity.c 

list.o: list.c
	$(CC) $(CFLAGS) -c list.c 

json.o: json.c
	$(CC) $(CFLAGS) -c json.c

game.o: game.c
	$(CC) $(CFLAGS) -c game.c

field.o: field.c
	$(CC) $(CFLAGS) -c field.c

clean:
	rm *.o server
