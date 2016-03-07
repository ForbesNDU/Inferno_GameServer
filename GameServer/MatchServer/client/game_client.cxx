#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT "13000"

int main (int argc, char **argv) {

	int sock;
	const char* hostname = "Toaster.dhcp.nd.edu"; 

	// Fetch addressing info
	struct addrinfo hints;
	struct addrinfo *serverinfo;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	int errval = getaddrinfo(hostname, PORT, &hints, &serverinfo);
	if(errval != 0) {
		std::cout << "Error getting addressing info: " << gai_strerror(errval) << std::endl;
		return EXIT_FAILURE;
	}
	
	// Create socket
	struct addrinfo *i;
	for(i = serverinfo; i != NULL; i = i->ai_next) {
	
		sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
		if(sock < 0) {
			continue;
		}

		break;
	
	} 

	// Verify and clean up
	if(i == NULL) {
		std::cout << "Unable to create socket" << std::endl;
	}

	freeaddrinfo(serverinfo);

	// Send packet every 17ms
	const char* buffer = "Q_ATTACK";

	int sent = 0;
	struct timeval current, last;
	gettimeofday(&last, NULL);

	while (1) {

		gettimeofday(&current, NULL);
		int current_us = current.tv_sec * 1000000 + current.tv_usec;
		int last_us = last.tv_sec * 1000000 + last.tv_usec;

		if( (current_us - last_us) >= 17000 ) {

			last.tv_usec += 17000;
			int bytes = sendto(sock, buffer, strlen(buffer), 0, i->ai_addr, i->ai_addrlen);
			if(bytes < 0) {
				std::cout << "Error sending packet to server" << std::endl;
				return EXIT_FAILURE;
			}

			sent++;
			if(sent % 100 == 0) {
				std::cout << "Sent " << sent << " packets" <<std::endl;
			}

		}	

	}

	return EXIT_SUCCESS;

}











