// Object maintained by the server that holds client info //

#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>

#define TIMEOUT (10)

class Client {
	public:
		Client(const struct sockaddr*, socklen_t);
		void fetch_addr_info(struct sockaddr*, socklen_t*) const;
		void update_timeout();
		bool is_timed_out() const;

	private:
		const struct sockaddr address;
		const socklen_t addrlen;
		struct timeval last_contact;

};

#endif
