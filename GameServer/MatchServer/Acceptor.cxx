#include "Acceptor.h"

Acceptor::Acceptor(uint16_t desired_port) {
	std::string portstr = boost::lexical_cast<std::string>( (int)desired_port );
	port = portstr.c_str();
} 

Acceptor::~Acceptor() {
	close(sock);
}

void Acceptor::attachConverterQueue(boost::lockfree::spsc_queue<struct packet, boost::lockfree::capacity<2000> > *conv_queue) {
	toConverter = conv_queue;
}

bool Acceptor::setup() {
	
	// Specify desired sock type
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	// Fetch local addressing info
	struct addrinfo *serverinfo;
	int errval = getaddrinfo( NULL, port, &hints, &serverinfo );
	if(errval != 0) {
		std::cout << "Error in getaddrinfo: " << gai_strerror(errval) << std::endl;
		return false;
	}

	// Attempt to find addr and bind to port
	struct addrinfo *i;
	for(i = serverinfo; i != NULL; i = i->ai_next) {

		sock = socket( i->ai_family, i->ai_socktype, i->ai_protocol );
		if(sock < 0) {
			continue;
		}

		errval = bind(sock, i->ai_addr, i->ai_addrlen);
		if(errval < 0) {
			close(sock);
			continue;
		}

		break;

	}

	// Check results and clean up
	if(i == NULL) {
		std::cout << "Unable to bind socket to local address" << std::endl;
		return false;
	}

	freeaddrinfo(serverinfo);
	
	return true;

}

void Acceptor::startReceiving() {

	std::cout << "Dispatch loop running..." << std::endl;

	// Receive loop
	struct sockaddr_storage currentClient;
	socklen_t addrlen = sizeof(currentClient);
	int packet_size;
	
	unsigned char buffer[MAXPACKETSIZE];
	memset(buffer, 0, sizeof(buffer));

	while(1) {

		packet_size = recvfrom(sock, buffer, MAXPACKETSIZE - 1, 0, (struct sockaddr*)&currentClient, &addrlen);				
		if(packet_size < 0) {
			std::cout << "Dropped packet: " << strerror(errno) << std::endl;
		}

		struct packet pack = buildpacket(currentClient, buffer);
		toConverter->push( pack );				

		std::cout << pack.data << std::endl;

	}	
	
}

struct packet Acceptor::buildpacket(struct sockaddr_storage addr, unsigned char* input_data) {

        struct packet abs_packet;

        abs_packet.address = addr;
        memcpy(abs_packet.data, input_data, MAXPACKETSIZE);

        return abs_packet;

}























