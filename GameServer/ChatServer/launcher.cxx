#include "Client.h"
#include "Session.h"
#include <stdlib.h>

int main(int argc, char **argv) {

	Session session(6, 3);
	session.start();

	return EXIT_SUCCESS;

}
