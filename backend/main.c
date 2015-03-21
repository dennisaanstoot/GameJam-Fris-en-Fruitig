#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libwebsockets.h>
#include <lws_config.h>


static int
my_protocol_callback(struct libwebsocket_context *context,
		struct libwebsocket *wsi,
		enum libwebsocket_callback_reasons reason,
		void *user, void *in, size_t len)
{
    
	char str[] = "start 400 300 1";
	size_t l;
	switch (reason) {
        // http://git.warmcat.com/cgi-bin/cgit/libwebsockets/tree/lib/libwebsockets.h#n260
	
	
	case LWS_CALLBACK_ESTABLISHED:
            printf("connection established\n");
      	    break;
	case LWS_CALLBACK_RECEIVE:
//	    char *msg = (char*) in;
	    printf("%s\n",(char*) in);
	    l = sizeof(str);
	    libwebsocket_write(wsi, str, l-1,0);
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
