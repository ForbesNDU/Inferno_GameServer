#ifndef LOGINSERVER_H
#define LOGINSERVER_H

// Native
#include <arpa/inet.h>
#include "boost/lexical_cast.hpp"
#include <cstring>
#include <ctime>
#include <errno.h>
#include <iostream>
#include <mutex>
#include <netdb.h>
#include "server_utils.h"
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// OpenSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rand.h>

// SQLConnector
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#define MAXPENDING (10)
#define MAXUNAMELEN (16)
#define TIMEOUT (5000000)

void handle_sigchild(int);

class LoginServer {
	public:
		LoginServer(uint16_t);
		~LoginServer();

		bool initialize();
		bool start();

	private:
		// Helper methods
		void log_connection_error(struct sockaddr_storage*);
		int get_request_type(int);

		// Login service routines
		bool login_user(int);
		bool chat_server_auth();
		bool match_server_auth();
	
		// Sign up service routines
		bool signup_user(int);		
		bool recv_password(int, char*);

		// OpenSSL routines
		SSL_CTX* fetch_ssl_context();
                DH* fetch_diffie_hellman_handle();
		bool rng_init();
		bool rng_get_AES_str(unsigned char*, size_t);
		bool ssl_initialize();

		// Internal server data
		const char* port;
		int next_registration_port;
		int next_login_port;	

		std::string SERVTAG;

		static int instance;
		FILE* logfile;
		int sock;

		std::mutex registration_port_mutex;
		std::mutex login_port_mutex;

		// MySQL Utilities
		sql::mysql::MySQL_Driver *driver;
		sql::Connection *con;
		
};

#endif
