#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include "Socket.h"

class UDP_Socket : public Socket {

	public:
		UDP_Socket(uint16_t);
		UDP_Socket(char*, uint16_t);
		~UDP_Socket();

                virtual void recv_str(char*, size_t, size_t*) = 0;
                virtual void recv_short(short*) = 0;

                virtual void send_str(char*, size_t) = 0;
                virtual void send_short(short) = 0;

	protected:
		bool initialize(init_code);

		bool initialize_server();
		bool initialize_client();

		int sock_fd;
		struct sockaddr last_addr;
		socklen_t last_addrlen;
		init_code initialized_with;

};

#endif
