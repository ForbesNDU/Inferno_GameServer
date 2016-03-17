#include "UDP_Socket.h"

////////// TOP LEVEL //////////

UDP_Socket::UDP_Socket(uint16_t port) : Socket(port) {

	bool is_initialized = initialize(INIT_AS_SERVER);
	if(!is_initialized) {
		throw INITIALIZE_EXCEPTION;
	}
	
}

UDP_Socket::UDP_Socket(char* hostname, uint16_t port) : Socket(hostname, port) {

	bool is_initialized = initialize(INIT_AS_CLIENT);
	if(!is_initialized) {
		throw INITIALIZE_EXCEPTION;
	}

}

UDP_Socket::~UDP_Socket() {
	close(sock_fd);
}

////////// PRIVATE METHODS //////////

bool UDP_Socket::initialize(init_code code) {

	initialized_with = code;

	if(code == INIT_AS_SERVER) {
		return initialize_server();
	} else {
		return initialize_client();
	}

}

bool UDP_Socket::initialize_server() {

	// Local addressing 
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *server_info;
	int errval = getaddrinfo(NULL, port, &hints, &server_info);
	if(errval != 0) {
		return false;		
	}

	// Bind to first available local addr
	struct addrinfo *i;
	for(i = server_info; i != NULL; i = i->ai_next) {
		
		sock_fd = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
		if(sock_fd == -1) {
			continue;
		}

		errval = bind(sock_fd, i->ai_addr, i->ai_addrlen);
		if(errval == -1) {
			close(sock_fd);
			continue;
		}

		break;

	} 

	freeaddrinfo(server_info);

	// Return results
	if(i == NULL) {
		return false;
	} else {
		return true;
	}

}

bool UDP_Socket::initialize_client() {

	// Local addressing
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
	
        struct addrinfo *server_info;
        int errval = getaddrinfo(remote_host, port, &hints, &server_info);
        if(errval != 0) {
                return false;
        }

	// Bind to first available remote addr
	struct addrinfo *i;
        for(i = server_info; i != NULL; i = i->ai_next) {

                sock_fd = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
                if(sock_fd == -1) {
                        continue;
                }

		break;

	}

	freeaddrinfo(server_info);

	// Set addressing members since we need to initialize comm as client
	last_addr = *(i->ai_addr);
	last_addrlen = i->ai_addrlen;

	// Return results
        if(i == NULL) {
                return false;
        } else {
                return true;
        }
	
}
