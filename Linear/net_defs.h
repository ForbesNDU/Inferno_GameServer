#ifndef NET_DEFS_H
#define NET_DEFS_H

#include <stdlib.h>
#include <string.h>


////////// MATCHMAKING //////////


typedef enum position {

	MAGIC_HYBRID,
	PHYSICAL_HYBRID,
	SUPPORT,
	GLASS_CANNON,
	JUGGERNAUT,
	FILL

} position;

typedef struct match_request {

	char token[64];
	char username[16];
	position pos;

} match_request;

match_request* build_match_request(char*, char*, position);

#endif
