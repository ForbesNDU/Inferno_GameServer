#include "server_utils.h"

void* get_in_addr(struct sockaddr *sa) {
        if(sa->sa_family == AF_INET) {
                return &(((struct sockaddr_in*)sa)->sin_addr);
        }
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

std::string hash_SHA_256( std::string input ) {

        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, input.c_str(), input.size());
        SHA256_Final(hash, &ctx);

        std::stringstream ss;

        int i;
        for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }

        return ss.str();

}


bool setup_TCP_connection(const char* hostname, const char* port, int *conn) {

        // Get server addressing info
        int sock;
	char server_addr[INET6_ADDRSTRLEN];

        struct addrinfo hints;
        struct addrinfo *serverinfo;

        memset( &hints, 0, sizeof(hints) );
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        int retval = getaddrinfo(hostname, port, &hints, &serverinfo);
        if(retval != 0) {
                return false;
        }

        // Connect to server
        struct addrinfo *i;
        for(i = serverinfo; i != NULL; i = i->ai_next) {

                sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
                if(sock == -1) {
                        continue;
                }

                retval = connect(sock, i->ai_addr, i->ai_addrlen);
                if(retval == -1) {
                        close(sock);
                        continue;
                }

                break;

        }

        // Check to ensure connection was established
        if(i == NULL) {
                return false;
        }

        inet_ntop(i->ai_family, get_in_addr((struct sockaddr*)i->ai_addr), server_addr, sizeof(server_addr));

        printf("Connected to server at: %s\n", server_addr);

        freeaddrinfo(serverinfo);

	// Return sock via ptr
	*conn = sock;

	return true;

}

bool setup_UDP_client_connection(const char* hostname, const char* port, int* sock, struct sockaddr *addr, socklen_t* len) {

        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;

        struct addrinfo *server_info;
        int errval = getaddrinfo(hostname, port, &hints, &server_info);
        if( errval != 0 ) {
                freeaddrinfo(server_info);
                return false;
        }

        struct addrinfo *i;
        for(i = server_info; i != NULL; i = i->ai_next) {
                *sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
                if(*sock == -1) {
                        continue;
                }

                break;
        }

        if(i == NULL) {
                freeaddrinfo(server_info);
                return false;
        }

        *addr = *(i->ai_addr);
        *len = i->ai_addrlen;

        freeaddrinfo(server_info);
        return true;

}

bool setup_UDP_server_connection(const char* port, int* sock) {

        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

        struct addrinfo *server_info;
        int errval = getaddrinfo(NULL, port, &hints, &server_info);
        if( errval != 0 ) {
                freeaddrinfo(server_info);
                return false;
        }

        struct addrinfo *i;
        for(i = server_info; i != NULL; i = i->ai_next) {

                *sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
                if(*sock == -1) {
                        continue;
                }

		errval = bind(*sock, i->ai_addr, i->ai_addrlen);
		if(errval == -1) {
			close(*sock);
			freeaddrinfo(server_info);
			continue;
		}

                break;

        }

        if(i == NULL) {
                freeaddrinfo(server_info);
                return false;
        }

	freeaddrinfo(server_info);
	return true;

}

std::string timestamp() {

        time_t current_time;
        struct tm* timeinfo;
        char buffer[100];

        time(&current_time);
        timeinfo = localtime(&current_time);

        strftime(buffer, 100, "[%d-%m-%Y %I:%M:%S CET]", timeinfo);
        std::string str(buffer);

        return str;

}









