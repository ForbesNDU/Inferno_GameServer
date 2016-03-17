#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <boost/lexical_cast.hpp>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <unordered_set>

typedef enum errorcode {
	PORT_EXCEPTION,
	INITIALIZE_EXCEPTION,
	BUFFER_OVERFLOW_EXCEPTION,
	RECEIVE_EXCEPTION,
	SEND_EXCEPTION
} errorcode;

typedef enum init_code {
	INIT_AS_SERVER,
	INIT_AS_CLIENT
} init_code;

class Socket {
	public:
		Socket(uint16_t);
		Socket(char*, uint16_t);

		virtual void recv_str(char*, size_t, size_t*) = 0;
		virtual void recv_short(short*) = 0;

		virtual void send_str(char*, size_t) = 0;
		virtual void send_short(short) = 0;

		static bool is_port_available(uint16_t);

	protected:
                virtual bool initialize(init_code) = 0;

		const char* remote_host;
		const char* port;	

	private:
                static std::unordered_set<uint16_t> open_ports;


};

#endif
