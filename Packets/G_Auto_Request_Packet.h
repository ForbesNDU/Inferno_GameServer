#ifndef G_AUTO_REQUEST_PACKET_H
#define G_AUTO_REQUEST_PACKET_H

#include "Packet.h"

struct auto_request {
	uint16_t target;
};

class G_Auto_Request_Packet : public Packet {
	public:
		G_Auto_Request_Packet(struct packet_header*, struct auto_request*);

	        void serialize(unsigned char*, size_t*) const;
                static void deserialize(unsigned char*, struct auto_request*);

                static size_t get_packet_size();	

	private:
		const struct auto_request auto_attack;		

};

#endif
