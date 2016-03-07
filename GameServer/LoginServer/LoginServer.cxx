#include "LoginServer.h"

int LoginServer::instance = 0;


//////// CONS AND DES ////////


LoginServer::LoginServer(uint16_t requested_port) {

	// Get port
	std::string port_string = boost::lexical_cast<std::string>( (int)requested_port );
	port = port_string.c_str();
	next_available_port = 13000;

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

	// Connect to users database
	con->setSchema("USERDB");

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

			bool didLogin = false;
	
			while(!didLogin) {			

				close(sock);
	
				RAND_poll();
	
				int req_type = get_request_type(data_conn);
				if(req_type == 0) { 
					didLogin = login_user(data_conn);
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

			}

			std::cout << "Closed auth session with " << client_addr_str << std::endl;

			close(data_conn);
			exit(0);

		}
		
	}

}


//////// SERVICE IMPLEMENTATIONS ////////


// Service Request Handler //
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

// Login Service //
bool LoginServer::login_user(int data_conn) {
	if(!recv_username(data_conn)) {
		return false;
	}

	if(!send_random(data_conn)) {
		return false;
	}

	if(!validate_response(data_conn)) {
		return false;
	}

	return true;
}

bool LoginServer::recv_username(int data_conn) {

	// Receive length of username
	short namelen;
	int bytes_received = recv(data_conn, &namelen, sizeof(short), 0);
	namelen = ntohs(namelen);

	// Receive username
	bytes_received = 0;
	char uname_buffer[MAXUNAMELEN];
	memset(uname_buffer, 0, MAXUNAMELEN * sizeof(char));

	while(bytes_received < namelen) {		
		bytes_received += recv(data_conn, uname_buffer, sizeof(uname_buffer), 0);
	}	

	std::cout << uname_buffer << std::endl;

	// Lookup username in database
	sql::ResultSet *res;
	sql::Statement *statement = con->createStatement();
	res = statement->executeQuery("SELECT username, password_hash FROM credentials ORDER BY username ASC");

	short success;
	if(!res->next()) {
		success = htons(1);
		std::cout << "Username not found." << std::endl;
		send(data_conn, &success, sizeof(short), 0);
		return false;	
	} else {
		success = htons(0);
		send(data_conn, &success, sizeof(short), 0);
		return true;		
	}

}

bool LoginServer::send_random(int data_conn) {

	// Get cryptographically strong random string
	short codelen = 256;
	unsigned char auth_code[codelen];
	memset(auth_code, 0, sizeof(auth_code));
	bool success = rng_get_AES_str(auth_code, codelen);	

	if(!success) {
		return false;
	} 

	// Send size of string to client
	codelen = htons(codelen);
	std::cout << codelen << std::endl;
	int bytes_sent = send(data_conn, &codelen, sizeof(short), 0);
	if(bytes_sent == -1) {
		std::cout << "Unable to send auth code length to client" << std::endl;
		return false;
	}

	// Send string to client
	bytes_sent = send(data_conn, auth_code, sizeof(auth_code), 0);
	if(bytes_sent == -1) {
		std::cout << "Unable to send auth code to client" << std::endl;
		return false;	
	}

	std::cout << "Auth code: "<< auth_code << std::endl;
	
	return true;

}

bool LoginServer::validate_response(int data_conn) {

	return true;

}

// Signup Service //
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
	/*
	// Add database entry
	sql::PreparedStatement *ps;
	
	ps = con->prepareStatement("INSERT INTO credentials(username, password_hash) VALUES(?, ?)");	
	ps->setString(1, uname_buffer);
	ps->setString(2, password);
	ps->execute();

	delete ps;

	// Acknowledge success	
	short success = htons(0);
	send(data_conn, &success, sizeof(short), 0);
	*/
	return true;

}

bool LoginServer::recv_password(int data_conn, char* password) {

	SSL_CTX* ctx = fetch_ssl_context();	
	
	// Get first free port and update
	// LOCK //
	port_mutex.lock();	
	// LOCK //

	std::string addr = boost::lexical_cast<std::string>(next_available_port);

	next_available_port += 1;
	if(next_available_port <= 20000) {
		next_available_port = 13000;
	}

	// UNLOCK //
	port_mutex.unlock();
	// UNLOCK //

	// Send port number to client
	short ssl_port = (short)std::stoi(addr);
	ssl_port = htons(ssl_port);
	int bytes = send(data_conn, & ssl_port, sizeof(short), 0);	
	if(bytes < 0) {
		fprintf(logfile, "%s Unable to send selected secure port to client\n", (timestamp()).c_str());
		return false;
	}

	std::cout << "Selected port: " << ntohs(ssl_port) << std::endl;

	// Assign port
	addr = "*:" + addr;
	std::cout << addr << std::endl;

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

	// Receive password (needs timeout/ async)
	char buffer[64];
	memset(buffer, 0, sizeof(buffer));

	int total_bytes = 0;
	int bytes_read;
	while(bytes_read = SSL_read(ssl, buffer + total_bytes, sizeof(buffer))) {
		
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

	// get & log address
}

void handle_sigchild(int s) {
	int last_error = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = last_error;
}


























