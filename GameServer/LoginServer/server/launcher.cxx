#include "LoginServer.h"

int main(int argc, char **argv) {

	LoginServer ls(7000);
	ls.initialize();
	ls.start();

	return 0;
}
