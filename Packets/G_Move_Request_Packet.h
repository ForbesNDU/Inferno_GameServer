#ifndef G_MOVE_REQUEST_PACKET_H
#define G_MOVE_REQUEST_PACKET_H

#include "Event_Packet.h"

struct move_request {
	uint32_t x;
	uint32_t y;
	uint32_t z;
};

class G_Move_Request_Packet : public Event_Packet {
	public:
		G_Move_Request_Packet(struct packet_header*, struct event_header*, struct move_request*);

                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct move_request*);

                static size_t get_packet_size();

	private:
		const struct move_request move;

};

#endif
