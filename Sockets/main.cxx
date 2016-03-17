#include "Unreliable_UDP_Socket.h"
#include <iostream>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	Unreliable_UDP_Socket sock(45001);
	std::cout << Socket::is_port_available(45000) << std::endl;

	return EXIT_SUCCESS;

}
