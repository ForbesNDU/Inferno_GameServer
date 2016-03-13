#include "ChatServer.h"


//////// TOP LEVEL ////////


ChatServer::ChatServer() {

	// Initialize authorized user data
	current_users = new std::map<std::string, std::string>();
	expiry = new std::deque<std::pair<std::string, long> >();

	// Setup logging
        std::string servtype ("CharServer");
        std::string extension (".log");
        std::string filename = servtype + extension;
        logfile = fopen(filename.c_str(), "a");

	// Connect to login server
	bool result = setup_UDP_client_connection("Toaster.dhcp.nd.edu", CHAT_PORT, &login_sock, &login_addr, &login_addrlen);
	if(!result) {
		fprintf(logfile, "%s Unable to connect to login server", (timestamp()).c_str());		
	}

}

ChatServer::~ChatServer() {

	delete current_users;
	delete expiry;

}


//////// PUBLIC METHODS ////////


void ChatServer::update() {

	while(1) {
		receive_new();
		scrub_list();
	}
	
}

bool ChatServer::create_session(Client** client_list, Session* session) {

	return true;	

}

//////// PRIVATE METHODS ////////

void ChatServer::scrub_list() {

	struct timeval tv;
	gettimeofday(&tv, 0);
	long current_time = (tv.tv_sec * 1000000) + tv.tv_usec;

	// Delete elements until expiry time has not been reached
	while( (expiry->front()).second < current_time ) {
	
		std::string username = (expiry->front()).first;
		expiry->pop_front();
		current_users->erase(username);

	}		
	
}

void ChatServer::receive_new() {

	// Initialize buffer space
	char buffer[85];
	char token_buffer[64];
	char uname_buffer[21];

	memset(buffer, 0, strlen(buffer));
	memset(token_buffer, 0, strlen(token_buffer));
	memset(uname_buffer, 0, strlen(uname_buffer));

	// Process all packets
	while( (recvfrom(login_sock, buffer, strlen(buffer), 0, &login_addr, &login_addrlen)) >= 0) {
	
		// Record token	
		memcpy(token_buffer, &buffer[0], 64);
		memcpy(uname_buffer, &buffer[64], 21);

		std::string token = boost::lexical_cast<std::string>(token_buffer);
		std::string username = boost::lexical_cast<std::string>(uname_buffer);

		current_users->insert( std::pair<std::string, std::string>(username, token) );

		// Record token expiry
		struct timeval tv;
		gettimeofday(&tv, 0);

		long expiry_time = (300 * 1000000) + ((tv.tv_sec) * 1000000) + tv.tv_usec;

		expiry->push_back( std::pair<std::string, long>(username, expiry_time) );

	}

}


















