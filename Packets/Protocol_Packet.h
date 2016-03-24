#ifndef PROTOCOL_PACKET_H
#define PROTOCOL_PACKET_H

#include "Packet.h"

typedef protocol_op enum : uint8_t {
	PING,
	SYN
} protocol_op;

struct protocol_header {
	protocol_op op
};

class Protocol_Packet : public Packet {
	public:
		Protocol_Packet();

                virtual void serialize(unsigned char*, size_t*) const = 0;
                static void deserialize(unsigned char**, struct packet_header*);

	private:
		// Utilities for subclasses to add general protocol packet header
                void add_header(unsigned char*) const;

                // General header contents
                const struct protocol_header pr_header;
                
};

#endif
