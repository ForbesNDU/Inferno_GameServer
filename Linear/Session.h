#ifndef SESSION_H
#define SESSION_H

#include <arpa/inet.h>
#include <boost/lexical_cast.hpp>
#include "Client.h"
#include <errno.h>
#include <iostream>
#include <list>
#include <netdb.h>
#include <netinet/in.h>
#include "server_utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXPACKETSIZE (1024)
#define SESSION_PORT "8001"

enum Team {
	RED,
	BLUE
};

class Session {
	public:
		Session(int, int);
		Session(int, int, std::list<Client*>*, std::list<Client*>*);
		~Session();

		bool addClient(Client*, enum Team);
		void removeClient(Client*);

		void start();

	private:
		// Helper Methods
		void enter_main_loop();
		void handle_accept();
		void handle_recv(int);

		// Logging / high-level
		FILE* logfile;
		static int count;
		int id;

		// Runtime constants
		const unsigned int MAXCLIENTS;
		const unsigned int TEAMSIZE;
		
		// Socket control
		int conn_socket;
		int max_fd;
		fd_set client_set;
		fd_set temp_set;

		// Client control
		std::list<Client*> redClients;
		std::list<Client*> blueClients;	

};

#endif
