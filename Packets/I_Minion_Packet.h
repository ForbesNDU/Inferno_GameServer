#ifndef I_MINION_PACKET_H
#define I_MINION_PACKET_H

#include "Sync_Packet.h"

struct minion_update {
	uint16_t id;
	uint16_t health;
	uint16_t max_health;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t rot_x;
	uint32_t rot_y;
	uint32_t rot_z;
	uint16_t auto_target;
};

class I_Minion_Packet : public Sync_Packet {
	public:
		I_Minion_Packet(struct packet_header*, struct sync_header*, struct minion_update*);

                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct minion_update*);

                static size_t get_packet_size();

	private:
		const struct minion_update minion_data;

};

#endif
