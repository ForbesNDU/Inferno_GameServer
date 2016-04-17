#ifndef G_AUTO_NOTIFY_PACKET_H
#define G_AUTO_NOTIFY_PACKET_H

#include "Event_Packet.h"

struct auto_notify {
	uint16_t target;
	uint16_t experience;
	uint16_t damage;
	uint8_t conf;
};

class G_Auto_Notify_Packet : public Event_Packet {
	public:
		G_Auto_Notify_Packet(struct packet_header*, struct event_header*, struct auto_notify*);
	
                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct auto_notify*);

                static size_t get_packet_size();

	private:
		const struct auto_notify auto_confirm;

};

#endif
