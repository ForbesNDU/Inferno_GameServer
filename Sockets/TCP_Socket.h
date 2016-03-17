#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "Socket.h"

class TCP_Socket : public Socket {
	public:
		TCP_Socket(char*);
		~TCP_Socket();

                void initialize(init_code);

                bool recv_str(char*, size_t*);
                bool recv_short(short*);

                bool send_str(char*, size_t);
                bool send_short(short);

	private:
		void initialize_server();
		void initialize_client();

};

#endif
