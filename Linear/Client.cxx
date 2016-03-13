#include "Client.h"

Client::Client(char* addr, char* name) :
	address(addr),
	username(name) {

}

Client::~Client() {

}

const char* Client::getAddress() {
	return address;
}

const char* Client::getUsername() {
	return username;
}
