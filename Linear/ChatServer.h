#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <arpa/inet.h>
#include <boost/lexical_cast.hpp>
#include "Client.h"
#include <deque>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include "server_utils.h"
#include "Session.h"
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>

#define CHAT_PORT "8000"

class ChatServer {
	public:
		ChatServer();
		~ChatServer();

		void update();
		bool create_session(Client**, Session*);

	private:
		// Tracks all authorized users
		std::map<std::string, std::string>* current_users;		
		std::deque<std::pair<std::string, long> >* expiry;

		// Update subroutines
		void receive_new();
		void scrub_list();

		struct sockaddr login_addr; 
		socklen_t login_addrlen;

		// File Descriptors
		int login_sock;
		FILE* logfile;

};

#endif
