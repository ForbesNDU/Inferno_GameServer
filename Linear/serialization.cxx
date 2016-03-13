#include "serialization.h"

void serialize_match_request(match_request* mr, char* buffer) {

	memcpy(buffer, mr->token, 64);

	char uname[16];
	memset(uname, 0, strlen(uname));
	memcpy(uname, mr->username, 16);
	memcpy(buffer + 64, uname, 16);
	
	sprintf(buffer + 80, "%d", mr->pos);

}

void deserialize_match_request(match_request* mr, char* buffer) {
	
	memcpy(mr->token, buffer, 64);
	memcpy(mr->username, buffer + 64, 16);
	mr->pos = atoi(buffer + 80);		

}
