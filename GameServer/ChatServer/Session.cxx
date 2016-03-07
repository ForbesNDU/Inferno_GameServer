#include "Session.h"

////////// LOGISTICS //////////

int Session::count = 0;

Session::Session(int maxClients, int teamSize, std::list<Client*> *redClients, std::list<Client*> *blueClients) :
	MAXCLIENTS(maxClients),
	TEAMSIZE(teamSize) {

	this->redClients = *redClients;
	this->blueClients = *blueClients;

	FD_ZERO(&client_set);
	FD_ZERO(&temp_set);

	id = count;
	count++;

        // Open log file
        std::string servtype("ChatSession");
        std::string extension (".log");
        std::string filename = servtype + boost::lexical_cast<std::string>(id) + extension;
        logfile = fopen(filename.c_str(), "a");

}

Session::Session(int maxClients, int teamSize) : 
	MAXCLIENTS(maxClients),
	TEAMSIZE(teamSize) {

	id = count;
	count++;

}

Session::~Session() {

}

bool Session::addClient(Client* newClient, enum Team team) {

	if( (team == RED) && (redClients.size() < TEAMSIZE) ) {
		redClients.push_back(newClient);
		return true;
	}
	else if( (team == BLUE) && (blueClients.size() < TEAMSIZE) ) {
		blueClients.push_back(newClient);
		return true;
	}
	else {
		return false;
	}

}

void Session::removeClient(Client* target) {

	redClients.remove(target);
	blueClients.remove(target);

} 

////////// NETWORKING //////////

void Session::start() {

	// Get local addressing info
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *serverinfo;
	int errval = getaddrinfo(NULL, PORT, &hints, &serverinfo);
	if(errval != 0) {
		fprintf(logfile, "Getaddrinfo error: %s\n", gai_strerror(errno));
		return;		
	}

	// Set up connection socket
	struct addrinfo *i;
	for( i = serverinfo; i != NULL; i = i->ai_next ) {
		
		conn_socket = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
		if(conn_socket < 0) {
			continue;
		}

		int yes = 1;
		errval = setsockopt(conn_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if(errval < 0) {
			fprintf(logfile, "Unable to set connection socket at reusable\n");
			return;
		}

		errval = bind(conn_socket, i->ai_addr, i->ai_addrlen);
		if(errval < 0) {
			close(conn_socket);
			continue;
		}

		break;

	}

	freeaddrinfo(serverinfo);

	if(i == NULL) {
		fprintf(logfile, "Unable to bind connection socket to local address\n");
		return;
	}

	// Attempt to listen on connection socket
	errval = listen(conn_socket, MAXCLIENTS);
	if(errval < 0) {
		fprintf(logfile, "Unable to listen on connection socket\n");
		return;
	}

	// Start chat session
	enter_main_loop();

}

void Session::enter_main_loop() {

	FD_SET(conn_socket, &client_set);
	max_fd = conn_socket;

	// Main Chat Loop
	while(1) {

		temp_set = client_set;
		int errval = select(max_fd + 1, &client_set, NULL, NULL, NULL);
		if(errval < 0) {
			fprintf(logfile, "Unable to use select\n");
			return;
		}

		int i;
		for(i = 0; i <= max_fd; i++) {
			if(FD_ISSET(i, &temp_set)) {
				
				if(i == conn_socket) {
					handle_accept();
				} else {
					handle_recv(i);
				}		

			}
		}

	}

}

void Session::handle_accept() {
	
	struct sockaddr clientaddr;
	socklen_t addrlen = sizeof(clientaddr);

	// Connect and handle errors
	int fd = accept(conn_socket, (struct sockaddr*)&clientaddr, &addrlen);
	if(fd < 0) {
		fprintf(logfile, "Error in accepting incoming onnection\n");
		return;
	}

	// Inform other clients
	FD_SET(fd, &client_set);
	if(fd > max_fd) {
		max_fd = fd;
	}

	char addr_str[INET6_ADDRSTRLEN];
	inet_ntop(clientaddr.sa_family, get_in_addr(&clientaddr), addr_str, sizeof(addr_str));

	std::cout << "Connection from: " << addr_str << std::endl;	

}

void Session::handle_recv(int sender_fd) {

	char buffer[MAXPACKETSIZE];
	memset(buffer, 0, sizeof(buffer));

	int bytes = recv(sender_fd, buffer, sizeof(buffer), 0);

	// Disconnection request handling
	if(bytes <= 0) {
		if(bytes == 0) {
			
		} else {

		}
		close(sender_fd);
		FD_CLR(sender_fd, &client_set);
	}

	// Data incoming
	else {

		int i;
		for(i = 0; i < max_fd; i++) {
			if(FD_ISSET(i, &client_set)) {
				if((i != conn_socket) && (i != sender_fd)) {
					send(i, buffer, bytes, 0);
				}
			}
		}

	}

}

































