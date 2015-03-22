#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libwebsockets.h>

#include <json/json.h>
#include <sys/time.h>

#include <pthread.h>

#include "field.h"
#include "list.h"
#include "game.h"
#include "json.h"
#include "entity.h"

char names[30][30];
int a[20];
unsigned player_amount;
struct game* game;
struct libwebsocket * sockets[2];

void* game_thread(void* args)
{
	int i;
	printf("Game on\n");
	unsigned char start[] = "start 800 600 2";
	size_t l = sizeof(start);
	
	for(i = 0; i < 2; i++)
	{
	    	libwebsocket_write(sockets[i], start, l-1,0);
	}
	while(!game_over(game))
	{
		game_tick(game);
		json_object* jobj = json_frame(game);
	
		const char * json_string = json_object_to_json_string(jobj);
		size_t size = strlen(json_string);
		char* string = malloc(size + 7);

		strcpy(string, "frame ");
		strcat(string,json_string);
		
		size = strlen(string);
		
		// printf("JSON: %s\n", string);
		
		for(i = 0; i < 2; i++)
		{
			libwebsocket_write(sockets[i],string, size, 0);
		}
		
		usleep(50000);
	}
	printf("End game\n");
	struct list * e_list = game->entity_list;
	for(i = 0; i < e_list->length; i++)
	{
		struct entity * e = e_list->array[i];
		switch(e->type) {
		case PLAYER:
			entity_player_destroy(e);
			break;
		case BULLET:
			entity_bullet_destroy(e);
			break;
		case TREE:
			entity_tree_destroy(e);
			break;
		}
	}
	game_destroy(game);
	game = NULL;
	pthread_exit(NULL);
}

struct list * build_entity_list()
{
	struct list* result;
	struct entity* e;
	result = malloc(sizeof(struct list));
	result->length = 0;
	result->size = 50;
	result->array = malloc(sizeof(void*) * result->size);
	int i;
	for(i = 0; i < 2; i++)
	{
		unsigned int x = rand() % 800;
		unsigned int y = rand() % 600;
		e = entity_player_new(x, y, 0 , 0, x, y, names[i]);
		list_add(result, e);
	}
	for(i = 0; i < 4; i++)
	{
		unsigned int x = rand() % 800;
		unsigned int y = rand() % 600;
		e = entity_tree_new(x, y);
		list_add(result, e);
	}
	return result;
}

static int
my_protocol_callback(struct libwebsocket_context *context,
		struct libwebsocket *wsi,
		enum libwebsocket_callback_reasons reason,
		void *user, void *in, size_t len)
{
	unsigned char ok[] = "ok";
    
	char* string;
	char* pch;
	size_t l;
	struct field field;
	struct list * entity_list;

	int i;

	unsigned int x;
	unsigned int y;
	char b;

	pthread_t thread;

	switch (reason) {
	
	case LWS_CALLBACK_ESTABLISHED:
            printf("Connection established\n");
      	    break;
	case LWS_CALLBACK_RECEIVE:
	    string = (char*) in;
	    
	    pch = strtok(string," ");


	    if(strncmp(pch,"connect",8) == 0)
	    {
		pch = strtok(NULL, " ");

		l = sizeof(ok);
		libwebsocket_write(wsi,ok,l-1,0);  

		strncpy(names[player_amount],pch,30);
		sockets[player_amount] = wsi;

		printf("Players: %d, %s\n", player_amount, names[player_amount]);

	        player_amount++;
		
		if(player_amount == 2)
		{
			field.width = 400;
			field.height = 300;
			entity_list = build_entity_list();
			game = game_new(field, entity_list);
			pthread_create(&thread, NULL, game_thread, NULL);
			player_amount = 0;
		}	

	    }
	    else if(strncmp(pch,"input",6) == 0 && game != NULL)
	    {
		pch = strtok(NULL, " ");
		b = pch[0];
		pch = strtok(NULL, " ");
		x = atoi(pch);
		pch = strtok(NULL, " ");
		y = atoi(pch);
		for(i = 0; i < 2; i++)
		{
			if(wsi == sockets[i])
			{
				struct list * e_list = game->entity_list;
				struct entity * e = e_list->array[i];
				struct player_info * info = (struct player_info *) e->info;
				if(b == 'l')
				{
					info->xd = x;
					info->yd = y;
				}
				else if(b == 'r')
				{
					entity_player_shoot(e,game,x,y);
				}
				break;
			}
		}
	    }

	    break;
        default:
//          printf("unhandled callback\n");
            break;
    	}
   
    	return 0;
}


int main(int argc, char** arg)
{
  // server url will be http://localhost:9000
  srand(time(NULL));
  player_amount = 0;
  int port = 9000;
  const char *interface = NULL;
  struct libwebsocket_context *context;
  // we're not using ssl
  const char *cert_path = NULL;
  const char *key_path = NULL;


  // no special options
  
  
  struct libwebsocket_protocols protocols[] = {
  {
	  	"fris_en_fruitig",
		my_protocol_callback,
		10000,
		100000,
		0,
		NULL,
		NULL,
		0
  	},
  	{NULL, NULL, 0, 0}
  };
  
  struct lws_context_creation_info info = {port, interface, protocols, libwebsocket_get_internal_extensions(), 
				    NULL, cert_path, key_path, NULL, 
				    NULL, NULL, NULL, 0,-1,-1, 0,
				    NULL, 0, 0, 0,
				    NULL 
  };
//  memset(&info, 0, sizeof info);
  context = libwebsocket_create_context(&info);
    
  if (context == NULL) {
    fprintf(stderr, "libwebsocket init failed\n");
    return -1;
  }
  printf("starting server...\n");
    
  // infinite loop, to end this server send SIGTERM. (CTRL+C)
  while (1) {
    libwebsocket_service(context, 50);
    // libwebsocket_service will process all waiting events with their
    // callback functions and then wait 50 ms.
    // (this is a single threaded webserver and this will keep our server
    // from generating load while there are not requests to process)
  }
    
  libwebsocket_context_destroy(context);
    
  return 0;
}
