#include "TCP_Socket.h"

////////// TOP LEVEL //////////

TCP_Socket::TCP_Socket(char* port) {
	super(port);
}

TCP_Socket::~TCP_Socket() {

}

////////// PUBLIC METHODS //////////

void TCP_Socket::initialize(init_code code) {
	if( code == INIT_AS_SERVER ) {
		initialize_server();
	} else {
		initialize_client();
	}
}

////////// PRIVATE METHODS //////////

void TCP_Socket::initialize_server() {

	// Get local addressing info
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        struct addrinfo *serverinfo;
        int errval = getaddrinfo(NULL, port, &hints, &serverinfo);
        if(errval != 0) {
                std::cout << "Getaddrinfo error: " << gai_strerror(errno) << std::endl;
                return;
        }

	// Setup primary connection socket
        struct addrinfo *i;
        for(i = serverinfo; i != NULL; i = i->ai_next) {

                sock_fd = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
                if(conn_socket < 0) {
                        continue;
                }

                int yes = 1;
                errval = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
                if(errval < 0) {
			close(sock_fd);
                        std::cout << "Unable to set socket at reusable" << std::endl;
                        return;
                }

                errval = bind(sock_fd, i->ai_addr, i->ai_addrlen);
                if(errval < 0) {
                        close(sock_fd);
                        continue;
                }

                break;

        }

        freeaddrinfo(serverinfo);

        if(i == NULL) {
                std::cout << "Unable to bind socket to local address" << std::endl;
                return;
        }
	

}

void TCP_Socket::initialize_client() {

}

