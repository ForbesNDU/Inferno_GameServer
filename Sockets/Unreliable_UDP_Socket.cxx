#include "Unreliable_UDP_Socket.h"

////////// TOP LEVEL //////////

Unreliable_UDP_Socket::Unreliable_UDP_Socket(uint16_t port) : UDP_Socket(port) {

}

Unreliable_UDP_Socket::Unreliable_UDP_Socket(char* hostname, uint16_t port) : UDP_Socket(hostname, port) {

}

////////// PUBLIC METHODS //////////

void Unreliable_UDP_Socket::recv_str(char* buffer, size_t buf_size, size_t* data_size) {

	int bytes = recvfrom(sock_fd, buffer, buf_size, 0, &last_addr, &last_addrlen);
	if(bytes == -1) {
		throw RECEIVE_EXCEPTION;
	}	
	
	if(bytes > (int)buf_size) {
		throw BUFFER_OVERFLOW_EXCEPTION;
	}
	*data_size = bytes;

}

void Unreliable_UDP_Socket::recv_short(short* data) {

	int bytes = recvfrom(sock_fd, data, sizeof(short), 0, &last_addr, &last_addrlen);
	if(bytes != 2) {
		throw RECEIVE_EXCEPTION;
	}
	*data = ntohs(*data);

}

void Unreliable_UDP_Socket::send_str(char* buffer, size_t buf_size) {
	
	int bytes = sendto(sock_fd, buffer, buf_size, 0, &last_addr, last_addrlen);	
	if(bytes != (int)buf_size) {
		throw SEND_EXCEPTION;
	}

}

void Unreliable_UDP_Socket::send_short(short data) {

	data = htons(data);
	int bytes = sendto(sock_fd, &data, sizeof(short), 0, &last_addr, last_addrlen);
	if(bytes != 2) {
		throw SEND_EXCEPTION;
	}

}





























