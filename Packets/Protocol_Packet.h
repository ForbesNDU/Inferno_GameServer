#ifndef PROTOCOL_PACKET_H
#define PROTOCOL_PACKET_H

#include "Packet.h"

typedef enum protocol_op : uint8_t {
	PING,
	SYN
} protocol_op;

struct protocol_header {
	protocol_op op;
};

class Protocol_Packet : public Packet {
	public:
		Protocol_Packet(struct packet_header*, struct protocol_header*);

                virtual void serialize(unsigned char*) const = 0;
                static void deserialize(unsigned char**, struct protocol_header*);

	protected:
		// Utilities for subclasses to add general protocol packet header
                void add_header(unsigned char*) const;
		static size_t get_header_size();

	private:
                // General header contents
                const struct protocol_header pr_header;
                
};

#endif
