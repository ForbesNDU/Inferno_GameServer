#include "LoginServer.h"

int LoginServer::instance = 0;


//////// CONS AND DES ////////


LoginServer::LoginServer(uint16_t requested_port) {

	// Get port
	std::string port_string = boost::lexical_cast<std::string>( (int)requested_port );
	port = port_string.c_str();
	next_registration_port = 13000;
	next_login_port = 14000;

	// Init debug string
	instance++;

	std::string servtype ("LoginServer");
	std::string ending(": ");
	SERVTAG = servtype + boost::lexical_cast<std::string>(instance) + ending;

	// Open logfile
	std::string extension (".log");
	std::string filename = servtype + boost::lexical_cast<std::string>(instance) + extension;
	logfile = fopen(filename.c_str(), "a");

	// Initialize SSL / RNG
	ssl_initialize();
	rng_init();

	// Connect to mySQL
	driver = sql::mysql::get_mysql_driver_instance();
	std::string password;
	std::cout << "Attempting to start user services. Please enter credentials: ";
	std::cin >> password;
	con = driver->connect("tcp://127.0.0.1:3306", "root", password);
	con->setSchema("USERDB");

	// UDP socket to chat server 
        const char* hostname = "Toaster.dhcp.nd.edu";
        const char* port = "8000";
        bool result = setup_UDP_connection(hostname, port, &chat_sock, &chat_addr, &chat_addrlen);
        if(!result) {
                std::cout << "Failed to initialize chat server connection" << std::endl;
        }

	// UDP socket to match server
        port = "30000";
        result = setup_UDP_connection(hostname, port, &match_sock, &match_addr, &match_addrlen);
        if(!result) {
        	std::cout << "Failed to initialize match server connection" << std::endl;      
        }

	// Set socks non-blocking
        fcntl(chat_sock, F_SETFL, O_NONBLOCK);
        fcntl(match_sock, F_SETFL, O_NONBLOCK);

}

LoginServer::~LoginServer() {
	close(sock);
	fclose(logfile);

	delete con;
}


//////// OPENSSL UTILITIES ////////

SSL_CTX* LoginServer::fetch_ssl_context() {

	// Set up context with Diffie-Hellman key
	SSL_CTX* ctx;

	ctx = SSL_CTX_new(TLSv1_server_method());
	if(!ctx) {
		fprintf(logfile, "%s Unable to create new context (SSL)\n", (timestamp()).c_str());
		return NULL;
	}
	
	DH* dh = fetch_diffie_hellman_handle();
	if(!dh) {
		return NULL;
	}
	
	SSL_CTX_set_tmp_dh(ctx, dh);

	if(SSL_CTX_set_cipher_list(ctx, "ADH-AES256-SHA") != 1) {
		fprintf(logfile, "%s No available cipher for DH exchange (SSL)\n", (timestamp()).c_str());
		return NULL;
	}

	return ctx;

}

DH* LoginServer::fetch_diffie_hellman_handle() {

	// Retrieve DH key pair
	DH* dh = DH_new();
	if(!dh) {
		fprintf(logfile, "%s Unable to create new DH struct (SSL)\n", (timestamp()).c_str());	
		return NULL;
	}

	if(!DH_generate_parameters_ex(dh, 2, DH_GENERATOR_2, 0)) {
		fprintf(logfile, "%s Failed to generate DH parameters (SSL)\n", (timestamp()).c_str());
		return NULL;
	}	

	int code = 0;
	if(!DH_check(dh, &code)) {
		fprintf(logfile, "%s DH check failed (SSL)\n", (timestamp()).c_str());
		return NULL;
	}

	if(!DH_generate_key(dh)) {
		fprintf(logfile, "%s Failed to generate DH key (SSL)\n", (timestamp()).c_str());
		return NULL;
	}

	return dh;

}

bool LoginServer::ssl_initialize() {

	// Initialize SSL library
	if(!SSL_library_init()) {
		return false;
	}
	SSL_load_error_strings();

	return true;

}

bool LoginServer::rng_init() {

	// Verify that the RNG is seeded
	RAND_poll();
	int is_seeded = RAND_status();
	if(is_seeded == 0) {
		std::cout << "Failed to seed the random number generator" << std::endl;
		return false;
	}

	return true;
	
}

bool LoginServer::rng_get_AES_str(unsigned char* buffer, size_t buflen) {

	if(buflen < 256) {
		std::cout << SERVTAG << "Requested string doesn't meet size requirement (256 bytes)" << std::endl;
		return false;
	}

	int retval = RAND_bytes(buffer, buflen);
	unsigned long errcode = ERR_get_error();

	if(retval != 1) {
		std::cout << SERVTAG << "Unable to generate random string" << std::endl;
		fprintf(logfile, "%s RNG Error: %lu", (timestamp()).c_str(), errcode);
		return false;
	}

	return true;

}


//////// SERVER INTERFACE ////////


bool LoginServer::initialize() {

	// Set up addressing info
	struct addrinfo addr_hint;
	struct addrinfo *available_addrs;
	memset( &addr_hint, 0, sizeof(addr_hint) );
	addr_hint.ai_family = AF_UNSPEC;
	addr_hint.ai_socktype = SOCK_STREAM;
	addr_hint.ai_flags = AI_PASSIVE;

	// Have the OS fill out the rest of the data
	int errval = getaddrinfo(NULL, port, &addr_hint, &available_addrs);
	if(errval != 0) {
		std::cout << SERVTAG << "Server init error: " << gai_strerror(errval) << std::endl;
		return false;
	}

	// Select an appropriate local address
	struct addrinfo *i;
	for( i = available_addrs; i != NULL; i = i->ai_next ) {
	
		sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
		if(sock == -1) {
			continue;
		}

		int yes = 1;
		errval = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if(errval == -1) {
			std::cout << SERVTAG << "Could not set address to reusable mode" << std::endl;
			return false;
		}

		errval = bind(sock, i->ai_addr, i->ai_addrlen);
		if(errval == -1) {
			close(sock);
			continue;
		}
		
		break;
	
	}

	// Clean up and return result
        freeaddrinfo(available_addrs);
	
	if(i == NULL) {
		std::cout << SERVTAG << "Unable to bind socket to a local address" << std::endl;
		return false;
	} else {
		return true;
	}

}

bool LoginServer::start() {

	// Convert socket to server sock
	int errval = listen(sock, MAXPENDING);
	if(errval == -1) {
		std::cout << strerror(errno) << std::endl;
		std::cout << SERVTAG << "Error attempting to listen on established socket" << std::endl;
		return false;
	}

	// Set up utilities to kill dead processes
	struct sigaction sa;
	sa.sa_handler = handle_sigchild;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if(sigaction(SIGCHLD, &sa, NULL) == -1) {
		std::cout << SERVTAG << "Unable to set up multiprocessing" << std::endl;
		return false;		
	}

	struct sockaddr_storage client_addr;
	socklen_t size;
	int data_conn;
	char client_addr_str[INET6_ADDRSTRLEN];

	// PRIMARY SERVICE LOOP //
	while(1) {
		
		size = sizeof(client_addr);

		data_conn = accept( sock, (struct sockaddr*)&client_addr, &size );
		if(data_conn == -1) {
			log_connection_error(&client_addr);
			continue;
		}		

		inet_ntop( client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr), client_addr_str, sizeof(client_addr_str) );
		std::cout << SERVTAG << "Received connection from " << client_addr_str << std::endl;

		// Child block
		if(!fork()) {

			close(sock);

			RAND_poll();

			int req_type = get_request_type(data_conn);
			if(req_type == 0) { 
				login_user(data_conn);
			}
			else if(req_type == 1) {
				signup_user(data_conn);
			}
			else if(req_type == -1) {
				std::cout << SERVTAG << "An internal error occurred while processing the user's request" << std::endl;
				break;
			}
			else {
				std::cout << SERVTAG << "User issued invalid request code: " << req_type << std::endl;
			}

			std::cout << "Closed session with: " << client_addr_str << std::endl;

			close(data_conn);
			exit(0);

		}
		
	}

	return true;

}


//////// SERVICE HANDLER ////////


int LoginServer::get_request_type(int data_conn) {

	short request_code;
	int bytes_received = recv(data_conn, &request_code, sizeof(short), 0);

	// Handle recv errors
	if(bytes_received < 0) {
		fprintf(logfile, "%s Unable to receive client request\n", (timestamp()).c_str());

		short ACK = htons(0);
		send(data_conn, &ACK, sizeof(short), 0);

		return -1;
	}

	// Respond immediately with ACK to client to signal beginning of secure transfer
	short ACK = htons(1);
	int bytes_sent = send(data_conn, &ACK, sizeof(short), 0);
	if(bytes_sent < 0) {
		fprintf(logfile, "%s Error occured sending request ACK back to client\n", (timestamp()).c_str());
		return -1;
	}

	// Start handling request
	request_code = (int)ntohs(request_code);
	return request_code;

}


//////// LOGIN HANDLER ////////


bool LoginServer::login_user(int data_conn) {

        SSL_CTX* ctx = fetch_ssl_context();

        // Get first free port and update
        // LOCK //
        login_port_mutex.lock();
        // LOCK //

        std::string addr = boost::lexical_cast<std::string>(next_login_port);

        next_login_port += 1;
        if(next_login_port <= 14000) {
                next_login_port = 13000;
        }

        // UNLOCK //
        login_port_mutex.unlock();
        // UNLOCK //

        // Send port number to client
        short ssl_port = (short)std::stoi(addr);
        ssl_port = htons(ssl_port);
        int bytes = send(data_conn, & ssl_port, sizeof(short), 0);
        if(bytes < 0) {
                fprintf(logfile, "%s Unable to send selected secure port to client\n", (timestamp()).c_str());
                return false;
        }

        // Assign port
        addr = "*:" + addr;

        BIO *acceptor = BIO_new_accept((char*)addr.c_str());
        if(!acceptor) {
                fprintf(logfile, "%s Unable to create secure socket\n", (timestamp()).c_str());
                return false;
        }

        // Create accept socket
        if(BIO_do_accept(acceptor) <= 0) {
                fprintf(logfile, "%s Unable to listen on secure socket\n", (timestamp()).c_str());
                return false;
        }

	// Listen on accept socket
        if(BIO_do_accept(acceptor) <= 0) {
                fprintf(logfile, "%s Unable to listen on secure socket\n", (timestamp()).c_str());
                return false;
        }

        BIO *client = BIO_pop(acceptor);

        SSL* ssl = SSL_new(ctx);
        if(!ssl) {
                fprintf(logfile, "%s Unable to create ssl context\n", (timestamp()).c_str());
                return false;
        }

        // Set non-blocking and notify client that server is ready for connect
        SSL_set_bio(ssl, client, client);

        if(SSL_accept(ssl) <= 0) {
                fprintf(logfile, "Error occured setting up SSL connection\n");
                return false;
        }

        // Receive length of username
        short namelen;
        int bytes_received = SSL_read(ssl, &namelen, sizeof(short));
        namelen = ntohs(namelen);

        // Receive username
        bytes_received = 0;
        char uname_buffer[MAXUNAMELEN];
        memset(uname_buffer, 0, MAXUNAMELEN * sizeof(char));

        while(bytes_received < namelen) {
                bytes_received += SSL_read(ssl, uname_buffer, sizeof(uname_buffer));
        }

        // Lookup username in database
        std::string query = "SELECT username, password_hash FROM credentials WHERE username='" + boost::lexical_cast<std::string>(uname_buffer) + "'";
        sql::ResultSet *res;
        sql::Statement *statement = con->createStatement();
        res = statement->executeQuery( query );

        short success;
	std::string stored_hash;
        if(!res->next()) {
                success = htons(1);
                std::cout << "Username not found." << std::endl;
                send(data_conn, &success, sizeof(short), 0);
                return false;
        } else {
                success = htons(0);
                send(data_conn, &success, sizeof(short), 0);
 		stored_hash = res->getString("password_hash");
        }

	std::cout << "Stored Hash: " << stored_hash << std::endl;

        // Receive password length
        short pass_len;
        SSL_read(ssl, &pass_len, sizeof(short));
        pass_len = ntohs(pass_len);

        // Receive password (needs timeout/ async)
        char buffer[64];
        memset(buffer, 0, sizeof(buffer));

        int total_bytes = 0;
        int bytes_read;
        while(total_bytes < pass_len) {
                bytes_read = SSL_read(ssl, buffer + total_bytes, sizeof(buffer));
                total_bytes += bytes_read;
        }

	// Hash password and compare
	std::string password = boost::lexical_cast<std::string>(buffer);
	std::string client_hash = hash_SHA_256(password);		

	std::cout << "Client Hash: " << client_hash << std::endl;

	if( client_hash.compare(stored_hash) != 0 ) {
		success = htons(1);
                std::cout << "Incorrect password" << std::endl;
                send(data_conn, &success, sizeof(short), 0);
                return false;
	} else {
		success = htons(0);
                send(data_conn, &success, sizeof(short), 0);
	}

	// Upon success, send auth string to client 
	unsigned char randstr[256];
	bool result = rng_get_AES_str(randstr, 256);
	if(!result) {
		return false;
	} 

	std::string token = hash_SHA_256( boost::lexical_cast<std::string>(randstr) );
	SSL_write(ssl, token.c_str(), strlen(token.c_str()));

	std::cout << "Auth token: " << token << std::endl;

        // Cleanup
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        BIO_free(acceptor);

        return true;


}


//////// LOCAL AUTHENTICATION ////////


bool LoginServer::chat_server_auth(char* auth_token, char* username) {

	// Build string
	char contents[85];
	memset(contents, 0, strlen(contents));

	strcpy(contents, auth_token);
	strcat(contents, username);

	// Send out auth string
	int bytes = sendto(chat_sock, contents, strlen(contents), 0, &chat_addr, chat_addrlen);
	if(bytes <= 0) {
		fprintf(logfile, "%s Failed to send auth token to chat server", (timestamp()).c_str());
		return false;
	}

	// Try to receive ACK
	int tries = 0;
	int elapsed_time = 0;
	short val;

	struct timeval t0, t1;
	gettimeofday(&t0, 0);

	while(1) {

		bytes = recvfrom(chat_sock, &val, sizeof(short), 0, &chat_addr, &chat_addrlen);
		if(bytes > 0) {
			break;
		}

		gettimeofday(&t1, 0);
		elapsed_time = ((t1.tv_sec - t0.tv_sec) * 1000000) + (t1.tv_usec - t0.tv_usec);

		if(elapsed_time > 500000) {
			tries += 1;
			bytes = sendto(chat_sock, contents, strlen(contents), 0, &chat_addr, chat_addrlen);
        	
			if(bytes <= 0) {
        		        fprintf(logfile, "%s Failed to send auth token to chat server", (timestamp()).c_str());
        		        return false;
		        }
		
			gettimeofday(&t0, 0);
		}

		if(tries > 10) {
			fprintf(logfile, "%s Unable to send auth token to chat server for user: %s ", (timestamp()).c_str(), username);
			return false;
		}

	} 

	return true;

}

bool LoginServer::match_server_auth(char* auth_token, char* username) {

        // Build string
        char contents[85];
        memset(contents, 0, strlen(contents));

        strcpy(contents, auth_token);
        strcat(contents, username);

        // Send out auth string
        int bytes = sendto(match_sock, contents, strlen(contents), 0, &match_addr, match_addrlen);
        if(bytes <= 0) {
                fprintf(logfile, "%s Failed to send auth token to chat server", (timestamp()).c_str());
                return false;
        }

        // Try to receive ACK
        int tries = 0;
        int elapsed_time = 0;
        short val;

        struct timeval t0, t1;
        gettimeofday(&t0, 0);
          
        while(1) {

                bytes = recvfrom(match_sock, &val, sizeof(short), 0, &match_addr, &match_addrlen);
                if(bytes > 0) {
                        break;
                }

                gettimeofday(&t1, 0);
                elapsed_time = ((t1.tv_sec - t0.tv_sec) * 1000000) + (t1.tv_usec - t0.tv_usec);

                if(elapsed_time > 500000) {
                        tries += 1;
                        bytes = sendto(match_sock, contents, strlen(contents), 0, &match_addr, match_addrlen);
                        
                        if(bytes <= 0) {
                                fprintf(logfile, "%s Failed to send auth token to chat server", (timestamp()).c_str());
                                return false;
                        }

                        gettimeofday(&t0, 0);
                }

                if(tries > 10) {
                        fprintf(logfile, "%s Unable to send auth token to chat server for user: %s ", (timestamp()).c_str(), username);
                        return false;
                }

        }

        return true;


} 


//////// SIGNUP HANDLER ////////


bool LoginServer::signup_user(int data_conn) {

        // Receive length of username
        short namelen;
        int bytes_received = recv(data_conn, &namelen, sizeof(short), 0);
        namelen = ntohs(namelen);

	std::cout << "Username length: " << namelen << std::endl;

        // Receive username 
        bytes_received = 0;
        char uname_buffer[MAXUNAMELEN];
        memset(uname_buffer, 0, MAXUNAMELEN * sizeof(char));

	struct timeval t0, t1;
	gettimeofday(&t0, NULL);
	long elapsed;

        while(bytes_received < namelen) {

                bytes_received += recv(data_conn, uname_buffer, sizeof(uname_buffer), 0);

		gettimeofday(&t1, NULL);
		elapsed = ((t1.tv_sec - t0.tv_sec) * 1000000) + (t1.tv_usec - t0.tv_usec);
		if(elapsed > TIMEOUT) {

			fprintf(logfile, "%s Create account request timeout\n", (timestamp()).c_str());
			short success = htons(-1);
			send(data_conn, &success, sizeof(success), 0);						

		}

        }	

	std::cout << "Username: " << uname_buffer << std::endl;

	char password[64];
	memset(password, 0, sizeof(password));

	// Receive password securely
	if(!recv_password(data_conn, password)) {
		return false;
	}
	
	// Hash password
	std::string hash = hash_SHA_256( boost::lexical_cast<std::string>(password) );
	
	// Add database entry
	short db_result_code;
	
	try {
		sql::PreparedStatement *ps;
	
		ps = con->prepareStatement("INSERT INTO credentials(username, password_hash) VALUES(?, ?)");	
		ps->setString(1, uname_buffer);
		ps->setString(2, hash);
		ps->execute();
	
		delete ps;
			
		db_result_code = 0;
	} 
	catch (sql::SQLException &e) {
		db_result_code = 1;
		fprintf(logfile, "%s MySQL exception: %s\n", (timestamp()).c_str(), e.what());
	}

	// Acknowledge success	
	short success = htons(db_result_code);
	send(data_conn, &success, sizeof(short), 0);
	
	return true;

}

bool LoginServer::recv_password(int data_conn, char* password) {

	SSL_CTX* ctx = fetch_ssl_context();	
	
	// Get first free port and update
	// LOCK //
	registration_port_mutex.lock();	
	// LOCK //

	std::string addr = boost::lexical_cast<std::string>(next_registration_port);

	next_registration_port += 1;
	if(next_registration_port <= 14000) {
		next_registration_port = 13000;
	}

	// UNLOCK //
	registration_port_mutex.unlock();
	// UNLOCK //

	// Send port number to client
	short ssl_port = (short)std::stoi(addr);
	ssl_port = htons(ssl_port);
	int bytes = send(data_conn, & ssl_port, sizeof(short), 0);	
	if(bytes < 0) {
		fprintf(logfile, "%s Unable to send selected secure port to client\n", (timestamp()).c_str());
		return false;
	}

	// Assign port
	addr = "*:" + addr;

	BIO *acceptor = BIO_new_accept((char*)addr.c_str());
	if(!acceptor) {
		fprintf(logfile, "%s Unable to create secure socket\n", (timestamp()).c_str());
		return false;
	}

	// Create accept socket
	if(BIO_do_accept(acceptor) <= 0) {
		fprintf(logfile, "%s Unable to listen on secure socket\n", (timestamp()).c_str());
		return false;
	}

	// Listen on accept socket
        if(BIO_do_accept(acceptor) <= 0) {
                fprintf(logfile, "%s Unable to listen on secure socket\n", (timestamp()).c_str());
                return false;
        }

	BIO *client = BIO_pop(acceptor);
	
	SSL* ssl = SSL_new(ctx);
	if(!ssl) {
		fprintf(logfile, "%s Unable to create ssl context\n", (timestamp()).c_str());
		return false;
	}

	// Set non-blocking and notify client that server is ready for connect
	SSL_set_bio(ssl, client, client);

	if(SSL_accept(ssl) <= 0) {
		fprintf(logfile, "Error occured setting up SSL connection\n");
		return false;
	}

	// Receive password length
	short pass_len;
	SSL_read(ssl, &pass_len, sizeof(short));
	pass_len = ntohs(pass_len);

	// Receive password (needs timeout/ async)
	char buffer[64];
	memset(buffer, 0, sizeof(buffer));

	int total_bytes = 0;
	int bytes_read;
	while(total_bytes < pass_len) {
		bytes_read = SSL_read(ssl, buffer + total_bytes, sizeof(buffer));
		total_bytes += bytes_read;
	}

	memcpy(password, buffer, sizeof(buffer));

	std::cout << "Received password: " << buffer << std::endl;
	
	// Cleanup
	SSL_shutdown(ssl);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	BIO_free(acceptor);
	
	return true;

}


//////// ADDITIONAL UTILITIES ////////


void LoginServer::log_connection_error(struct sockaddr_storage* client_info) {
	struct sockaddr *address = (struct sockaddr*)client_info;

	std::string protocol;
	if(address->sa_family == AF_INET) {
		protocol = "IPv4";	
	} 
	else if(address->sa_family == AF_INET6) {
		protocol = "IPv6";
	}
	else {
		std::cout << "Client tried to connect using unknown addressing protocol" << std::endl;
		return;
	}

}

void handle_sigchild(int s) {
	int last_error = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = last_error;
}


























