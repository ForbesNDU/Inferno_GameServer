#ifndef C_PING_PACKET_H
#define C_PING_PACKET_H

#include "Packet.h"

struct c_ping {
	uint8_t stamp;
};

class C_Ping_Packet : public Packet {
	public:
		C_Ping_Packet(struct packet_header*, struct c_ping*);

                void serialize(unsigned char*, size_t*) const;
                static void deserialize(unsigned char*, struct c_ping*);

                static size_t get_packet_size();

	private:
		const struct c_ping rtt;

};

#endif
