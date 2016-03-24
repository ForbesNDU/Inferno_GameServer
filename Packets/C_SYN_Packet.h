#ifndef C_SYN_PACKET_H
#define C_SYN_PACKET_H

#include "Packet.h"

typedef enum syn_type : uint8_t {
	CONNECT,
	DISCONNECT
};

struct syn_request {
	syn_type req;
	uint8_t conf_number;
};

class C_SYN_Packet : public Packet {
	public:
		C_SYN_Packet(struct packet_header*, struct syn_request*);
		
                void serialize(unsigned char*, size_t*) const;
                static void deserialize(unsigned char*, struct syn_request*);

                static size_t get_packet_size();

	private:
		const struct syn_request content;

};

#endif
