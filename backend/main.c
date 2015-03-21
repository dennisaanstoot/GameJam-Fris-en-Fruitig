#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libwebsockets.h>
#include <lws_config.h>

#include <json/json.h>
#include <sys/time.h>

#include <pthread.h>

#include "field.h"
#include "list.h"
#include "game.h"
#include "json.h"
#include "entity.h"

const char start[] = "start 400 300 1";

char names[4][30];
unsigned player_amount;
struct game* game;

/*

void* game_thread(void* args)
{
	while(!game_over(game))
	{
		game_tick(game);
		json_object* jobj = json_frame(game);
		
	}
	pthread_exit(NULL);
}

*/

struct list * build_entity_list()
{
	struct list* result;
	struct entity* e;
	result = malloc(sizeof(struct list));
	int i;
	for(i = 0; i < 4; i++)
	{
		unsigned int x = rand() % 400;
		unsigned int y = rand() % 300;
		e = entity_player_new(x, y, 0 , 0, x, y, names[i]);
		if(i == 0)
		{
			result->e = e;
		}
		else
		{
			list_add(result, e);
		}
	}
	return result;
}

static int
my_protocol_callback(struct libwebsocket_context *context,
		struct libwebsocket *wsi,
		enum libwebsocket_callback_reasons reason,
		void *user, void *in, size_t len)
{
	srand(time(NULL));
	player_amount = 0;
	char start[] = "start 400 300 4";
    
	char* string;
	char* pch;
	size_t l;
	struct field field;
	struct list * entity_list;

	unsigned int x;
	unsigned int y;

	switch (reason) {
	
	case LWS_CALLBACK_ESTABLISHED:
            printf("connection established\n");
      	    break;
	case LWS_CALLBACK_RECEIVE:
	    string = (char*) in;
	    
	    pch = strtok(string," ");

	    if(strncmp(pch,"connect",8))
	    {
		pch = strtok(string, " ");

		strncpy(names[player_amount],pch,30);
	        player_amount++;

		if(player_amount == 4)
		{
			field.width = 400;
			field.height = 300;
			entity_list = build_entity_list();
			game = game_new(field, entity_list);

		}	

		l = sizeof(start); 
	    	libwebsocket_write(wsi, start, l-1,0);
	    }
	    else if(strncmp(pch,"input",6))
	    {
		pch = strtok(string, " ");
		x = atoi(pch);
		pch = strtok(string, " ");
		y = atoi(pch);
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
		100,
		100,
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
