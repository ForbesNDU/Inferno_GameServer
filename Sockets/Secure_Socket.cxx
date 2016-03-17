#include "Secure_Socket.h"

////////// TOP LEVEL //////////

Secure_Socket::Secure_Socket(uint16_t req_port) : Socket(req_port) {

	bool is_initialized = initialize(INIT_AS_SERVER);
	if(!is_initialized) {
		throw INITIALIZE_EXCEPTION;
	}

}

Secure_Socket::Secure_Socket(char* hostname, uint16_t req_port) : Socket(hostname, req_port) {

	bool is_initialized = initialize(INIT_AS_CLIENT);
	if(!is_initialized) {
		throw INITIALIZE_EXCEPTION;
	}

}

Secure_Socket::~Secure_Socket() {

	SSL_shutdown(ssl);
	SSL_free(ssl);
	SSL_CTX_free(ctx);

}

////////// PUBLIC METHODS //////////

void Secure_Socket::recv_str(char* buffer, size_t buf_size, size_t* data_size) {

}

void Secure_Socket::recv_short(short* data) {

}

void Secure_Socket::send_str(char* buffer, size_t buf_size) {

}

void Secure_Socket::send_short(short data) {

}

////////// PRIVATE METHODS //////////

bool Secure_Socket::initialize(init_code code) {

	if(!ssl_initialize()) {
		return false; 
	}	

	ctx = fetch_ssl_context();
	if(code == INIT_AS_SERVER) {
		return initialize_server();
	} else {
		return initialize_client();
	}	

}

DH* Secure_Socket::fetch_diffie_hellman_handle() {

	DH* dh = DH_new();
	if(!dh) {
		return NULL;
	}

	if(!DH_generate_parameters_ex(dh, 2, DH_GENERATOR_2, 0)) {
		return NULL;
	}	

	int code = 0;
	if(!DH_check(dh, &code)) {
		return NULL;
	}

	if(!DH_generate_key(dh)) {
		return NULL;
	}

	return dh;

}

SSL_CTX* Secure_Socket::fetch_ssl_context() {

	SSL_CTX* ctx;

	ctx = SSL_CTX_new(TLSv1_server_method());
	if(!ctx) {
		return NULL;
	}
	
	DH* dh = fetch_diffie_hellman_handle();
	if(!dh) {
		return NULL;
	}
	
	SSL_CTX_set_tmp_dh(ctx, dh);

	if(SSL_CTX_set_cipher_list(ctx, "ADH-AES256-SHA") != 1) {
		return NULL;
	}

	return ctx;

}

bool Secure_Socket::ssl_initialize() {

	if(!SSL_library_init()) {
		return false;
	}
	SSL_load_error_strings();

	return true;

}

bool Secure_Socket::initialize_server() {

        std::string addr = "*:" + addr;

        acceptor = BIO_new_accept((char*)addr.c_str());
        if(!acceptor) {
                return false;
        }	

        if(BIO_do_accept(acceptor) <= 0) {
                return false;
        }

        if(BIO_do_accept(acceptor) <= 0) {
                return false;
        }

        BIO *client = BIO_pop(acceptor);

        ssl = SSL_new(ctx);
        if(!ssl) {
                return false;
        }

        SSL_set_bio(ssl, client, client);

        if(SSL_accept(ssl) <= 0) {
                return false;
        }

	return true;

}

bool Secure_Socket::initialize_client() {

	std::string addr = boost::lexical_cast<std::string>(remote_host) + ":" + boost::lexical_cast<std::string>(port);
	
        BIO* dispatcher = BIO_new_connect((char*)addr.c_str());
        if(!dispatcher) {
                return false;
        }

        if(BIO_do_connect(dispatcher) <= 0) {
                return false;
        }

        ssl = SSL_new(ctx);
        if(!ssl) {
                return false;
        }

        SSL_set_bio(ssl, dispatcher, dispatcher);

        if(SSL_connect(ssl) <= 0) {
                return false;
        }

	return true;

}



















