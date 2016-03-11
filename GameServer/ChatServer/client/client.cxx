// Client test code for Chat Server //
// Peter Forbes - 3/10/2016 //

#include <iostream>
#include "server_utils.h"

int main (void) {

	// Connect to chat server
	int sock;
	const char* hostname = "Toaster.dhcp.nd.edu";
	const char* port = "8000";

	bool result = setup_TCP_connection(hostname, port, &sock);
	if(result == false) {
		std::cout << "Failed to initialize connection to chat server" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}
