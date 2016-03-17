#ifndef SECURE_SOCKET_H
#define SECURE_SOCKET_H

#include "Socket.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

class Secure_Socket : public Socket {
	public:
		Secure_Socket(uint16_t);
		Secure_Socket(char*, uint16_t);
		~Secure_Socket();

                void recv_str(char*, size_t, size_t*);
                void recv_short(short*);

                void send_str(char*, size_t);
                void send_short(short);

	private:
		bool initialize(init_code);
		bool initialize_server();
		bool initialize_client();

		bool ssl_initialize();
		DH* fetch_diffie_hellman_handle();
		SSL_CTX* fetch_ssl_context();

		SSL_CTX* ctx;
		SSL* ssl;
		BIO* acceptor;

};

#endif
