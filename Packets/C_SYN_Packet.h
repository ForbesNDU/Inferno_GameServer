#ifndef C_SYN_PACKET_H
#define C_SYN_PACKET_H

#include "Protocol_Packet.h"

typedef enum syn_type : uint8_t {
	CONNECT,
	DISCONNECT
} syn_type;

struct syn_request {
	syn_type req;
	uint8_t conf_number;
};

class C_SYN_Packet : public Protocol_Packet {
	public:
		C_SYN_Packet(struct packet_header*, struct protocol_header*, struct syn_request*);
		
                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct syn_request*);

                static size_t get_packet_size();

	private:
		const struct syn_request syn_content;

};

#endif
