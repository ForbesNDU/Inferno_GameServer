#ifndef C_PING_PACKET_H
#define C_PING_PACKET_H

#include "Protocol_Packet.h"

struct c_ping {
	uint8_t stamp;
};

class C_Ping_Packet : public Protocol_Packet {
	public:
		C_Ping_Packet(struct packet_header*, struct protocol_header*, struct c_ping*);

                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct c_ping*);

                static size_t get_packet_size();

	private:
		const struct c_ping rtt;

};

#endif
