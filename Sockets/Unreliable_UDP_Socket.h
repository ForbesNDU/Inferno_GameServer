#ifndef UNRELIABLE_UDP_SOCKET_H
#define UNRELIABLE_UDP_SOCKET_H

#include "UDP_Socket.h"

class Unreliable_UDP_Socket : public UDP_Socket {
	public:
		Unreliable_UDP_Socket(uint16_t);
		Unreliable_UDP_Socket(char*, uint16_t);

                void recv_str(char*, size_t, size_t*);
                void recv_short(short*);

                void send_str(char*, size_t);
                void send_short(short);

};

#endif
