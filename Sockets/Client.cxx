#include "Client.h"

Client::Client(const struct sockaddr* addr, socklen_t length) : 
	address(*addr),
	addrlen(length) {

	gettimeofday(&last_contact, 0);	

}

void Client::fetch_addr_info(struct sockaddr* addr, socklen_t* len) const {
	*addr = address;
	*len = addrlen;
}

void Client::update_timeout() {
	gettimeofday(&last_contact, 0);
}

bool Client::is_timed_out() const {

	struct timeval current_time;
	gettimeofday(&current_time, 0);

	unsigned int elapsed = current_time.tv_sec - last_contact.tv_sec;
	if(elapsed >= TIMEOUT) {
		return true;
	} else {
		return false;
	}

}
