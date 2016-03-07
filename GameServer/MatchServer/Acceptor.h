#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <arpa/inet.h>
#include <boost/lexical_cast.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include "netdefs.h"
#include <netinet/in.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXPACKETSIZE (1024)

class Acceptor {
	public:
		Acceptor(uint16_t);
		~Acceptor();

		void attachConverterQueue( boost::lockfree::spsc_queue<struct packet, boost::lockfree::capacity<2000> >* );
		bool setup();
		void startReceiving();

	private:
		struct packet buildpacket(struct sockaddr_storage, unsigned char*);

		boost::lockfree::spsc_queue<struct packet, boost::lockfree::capacity<2000> > *toConverter;

		const char *port;
		int sock;

};

#endif
