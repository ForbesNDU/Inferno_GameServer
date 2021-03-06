#ifndef G_MOVE_NOTIFY_PACKET_H
#define G_MOVE_NOTIFY_PACKET_H

#include "Event_Packet.h"

struct move_notify {
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint8_t conf;
};

class G_Move_Notify_Packet : public Event_Packet {
	public:
		G_Move_Notify_Packet(struct packet_header*, struct event_header*, struct move_notify*);

                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct move_notify*);

                static size_t get_packet_size();

	private:
		const struct move_notify move;

};

#endif
