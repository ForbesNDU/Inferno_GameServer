#include "ChatServer.h"
#include "LoginServer.h"
#include "MatchMaker.h"

#include <stdlib.h>

int main(void) {

	MatchMaker mm;
	ChatServer cs;
	LoginServer ls(7000);

	return EXIT_SUCCESS;

}
