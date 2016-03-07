#include <arpa/inet.h>
#include <boost/lexical_cast.hpp>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

char server_addr[INET6_ADDRSTRLEN];

////////// DEFINITIONS //////////


#define MAXNAMELEN (16)
#define LOGIN_PORT "7000"
#define AES_STR_LEN (256)
#define MTU (512)

void* get_in_addr(struct sockaddr*);
bool ssl_initialize();
SSL_CTX* fetch_ssl_context();

bool login(int);
bool signup(int);
bool change_password(int);


////////// PRIMARY CLIENT SERVICES //////////


int main(int argc, char **argv) {

	// Get server addressing info
	const char *hostname = "Toaster.dhcp.nd.edu";
	int sock;

	struct addrinfo hints;
	struct addrinfo *serverinfo;

	memset( &hints, 0, sizeof(hints) ); 
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int retval = getaddrinfo(hostname, LOGIN_PORT, &hints, &serverinfo);
	if(retval != 0) {
		printf("Error in fetching server address info: %s\n", gai_strerror(retval));
		return EXIT_FAILURE;
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
		printf("Error in connecting to server\n");
		return EXIT_FAILURE;
	}

	inet_ntop(i->ai_family, get_in_addr((struct sockaddr*)i->ai_addr), server_addr, sizeof(server_addr));

	printf("Connected to server at: %s\n", server_addr);

	freeaddrinfo(serverinfo);

	// Login Services Loop
	
	while(1) {

		std::string service_code;
		std::cout << std::endl << "Enter a valid service code (0, 1, 2): ";
		std::cin >> service_code;

		int code = atoi(service_code.c_str());

		if(code == 0) {
			login(sock);
		}
		else if(code == 1) {
			signup(sock);
		}
		else if(code == 2) {
			change_password(sock);
		}
		else {
			std::cout << "Invalid service code" << std::endl;
		}

	}

	close(sock);
	return EXIT_SUCCESS;

}


////////// IMPLEMENTATION DETAILS //////////


bool login(int sock) {

        // Get login info
        std::cout << std::endl << "--- ENTER CREDENTIALS ---" << std::endl << std::endl;

        std::string username;
        std::cout << "Username: ";
        std::cin >> username;

        std::string password;
        std::cout << "Password: ";
        std::cin >> password;

	// Send code
	short service_code = 0;
	service_code = htons(service_code);
	int bytes_sent = send(sock, &service_code, sizeof(short), 0);
	if(bytes_sent < 0) {
		std::cout << "Unable to send service code to server: " << strerror(errno) << std::endl;
		return false;
	}

	// Send username length
	short usernamelen = username.length();
	usernamelen = htons(usernamelen);
	bytes_sent = send(sock, &usernamelen, sizeof(short), 0);
        if(bytes_sent < 0) {
                std::cout << "Unable to send username length to server: " << strerror(errno) << std::endl;
                return false;
        }

	// Send username
	char uname[MAXNAMELEN];
	memset(uname, 0, MAXNAMELEN * sizeof(char));
	sprintf(uname, "%s", username.c_str());
	uname[strlen(uname)] = '\0';
	
	bytes_sent = send(sock, uname, sizeof(uname), 0);
	if(bytes_sent < 0) {
		std::cout << "Unable to send username to server: " << strerror(errno) << std::endl;
		return false;
	}	

	// Receive length of random string
        /*short authlen;
        int bytes_received = recv(sock, &authlen, sizeof(short), 0);
	//authlen = ntohs(authlen);

	// Receive random string
	char auth_str[authlen];
	std::cout << authlen << std::endl;
	memset(auth_str, 0, authlen * sizeof(char));

	bytes_received = 0;
	int block = 0;

	while(bytes_received < authlen) {

		block = recv(sock, &auth_str, sizeof(auth_str), 0);
		if(block < 0) {
			std::cout << "Error occurred when receiving rand string from server" << std::endl;
			return false;
		}
		bytes_received += block;

	}		

	std::cout << "Random string: " << auth_str << std::endl; 
	*/
	return true;
	
}

bool signup(int sock) {

        // Get login info
        std::cout << std::endl << "--- REGISTRATION ---" << std::endl << std::endl;

        std::string username;
        std::cout << "Username: ";
        std::cin >> username;

        std::string password;
        std::cout << "Password: ";
        std::cin >> password;

        // Send code
        short service_code = 1;
        service_code = htons(service_code);
        int bytes_sent = send(sock, &service_code, sizeof(short), 0);
        if(bytes_sent < 0) {
                std::cout << "Unable to send service code to server: " << strerror(errno) << std::endl;
                return false;
        }

        // Send username length
        short usernamelen = username.length();
        usernamelen = htons(usernamelen);
        bytes_sent = send(sock, &usernamelen, sizeof(short), 0);
        if(bytes_sent < 0) {
                std::cout << "Unable to send username length to server: " << strerror(errno) << std::endl;
                return false;
        }

        // Send username
        char uname[MAXNAMELEN];
        memset(uname, 0, MAXNAMELEN * sizeof(char));
        sprintf(uname, "%s", username.c_str());
        uname[strlen(uname)] = '\0';

        bytes_sent = send(sock, uname, sizeof(uname), 0);
        if(bytes_sent < 0) {
                std::cout << "Unable to send username to server: " << strerror(errno) << std::endl;
                return false;
        }

	// Receive port number 
	short ssl_port;
	int bytes_received = recv(sock, &ssl_port, sizeof(ssl_port), 0);
	if(bytes_received <= 0) {
		std::cout << "Failed to receive port number for ssl connection" << std::endl;
		return false;
	}

        bytes_received = recv(sock, &ssl_port, sizeof(ssl_port), 0);
        if(bytes_received <= 0) {
                std::cout << "Failed to receive port number for ssl connection" << std::endl;
                return false;
        }

	std::string addr =  "Toaster.dhcp.nd.edu:" + boost::lexical_cast<std::string>(ntohs(ssl_port));
	std::cout << "Secure addr: " << addr << std::endl;	

	// Send password over SSL
	if(!ssl_initialize()) {
		return false;
	} 

	SSL_CTX* ctx = fetch_ssl_context();
	
	std::cout << (char*)addr.c_str() << std::endl;
	BIO* dispatcher = BIO_new_connect((char*)addr.c_str());
	if(!dispatcher) {
		std::cout << "Unable to create connection (SSL)" << std::endl;
		return false;
	}

	if(BIO_do_connect(dispatcher) <= 0) {
		std::cout << "Unable to connect to server (SSL)" << std::endl;
		return false;
	}

	SSL* ssl = SSL_new(ctx);
	if(!ssl) {
		std::cout << "Unable to create context (SSL)" << std::endl;
		return false;
	}	

	SSL_set_bio(ssl, dispatcher, dispatcher);

	if(SSL_connect(ssl) <= 0) {
		std::cout << "Unable to bind socket to SSL" << std::endl;
		return false;
	}

	char* char_pass = (char*)password.c_str();
	SSL_write(ssl, char_pass, sizeof(char_pass));

	// Receive ACK

	// Shutdown
	SSL_shutdown(ssl);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	
	std::cout << "Successfully registered" << std::endl;
	
	return true;
	
}

bool change_password(int sock) {

	return true;

}

void* get_in_addr(struct sockaddr *sa) {

	if(sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);

}

bool ssl_initialize() {

	if(!SSL_library_init()) {
		std::cout << "Unable to initilize OpenSSL" << std::endl;
		return false;
	}
	SSL_load_error_strings();

	return true;

}

SSL_CTX* fetch_ssl_context() {

	SSL_CTX* ctx;
	ctx = SSL_CTX_new(TLSv1_client_method());
	if(!ctx) {
		std::cout << "Unable to create context (SSL)" << std::endl;
		return NULL;
	}

	if(SSL_CTX_set_cipher_list(ctx, "ADH-AES256-SHA") != 1) {
		std::cout << "Unable to set cipher list (SSL)" << std::endl;
		return NULL;
	}

	return ctx;

}






































