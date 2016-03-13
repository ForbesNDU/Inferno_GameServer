#include "net_defs.h"

void build_match_request(match_request* mr, char* auth_token, char* uname, position position_req) {

	memcpy(mr->token, auth_token, 64);
	memset(mr->username, 0, 16);
	memcpy(mr->username, uname, strlen(uname));
	mr->pos = position_req;	

}
