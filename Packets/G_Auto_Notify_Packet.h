#ifndef G_AUTO_NOTIFY_PACKET_H
#define G_AUTO_NOTIFY_PACKET_H

#include "Packet.h"

struct auto_notify {
	uint16_t target;
	uint16_t experience;
	uint16_t damage;
	uint8_t conf;
};

class G_Auto_Notify_Packet : public Packet {
	public:
		G_Auto_Notify_Packet(struct packet_header*, struct auto_notify*);
	
                void serialize(unsigned char*, size_t*) const;
                static void deserialize(unsigned char*, struct auto_notify*);

                static size_t get_packet_size();

	private:
		const struct auto_notify auto_confirm;

};

#endif
