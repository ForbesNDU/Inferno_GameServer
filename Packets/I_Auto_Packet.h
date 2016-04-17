#ifndef I_AUTO_PACKET_H
#define I_AUTO_PACKET_H

#include "Sync_Packet.h"

struct auto_update {
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint16_t owner;
	uint16_t target;
	uint16_t damage;
	uint8_t removed;
};

class I_Auto_Packet : public Sync_Packet {
	public:
		I_Auto_Packet(struct packet_header*, struct sync_header*, struct auto_update*);

                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct auto_update*);

                static size_t get_packet_size();

	private:
		const struct auto_update auto_data;

};

#endif
