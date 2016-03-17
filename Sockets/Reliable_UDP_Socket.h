#ifndef RELIABLE_UDP_SOCKET_H
#define RELIABLE_UDP_SOCKET_H

#include "UDP_Socket.h"

class Reliable_UDP_Socket : public UDP_Socket {
	public:
		Reliable_UDP_Socket(char*);
		~Reliable_UDP_Socket();

                bool recv_str(char*, size_t*) = 0;
                bool recv_short(short*) = 0;

                bool send_str(char*, size_t) = 0;
                bool send_short(short) = 0;

                void initialize() = 0;

};

#endif
