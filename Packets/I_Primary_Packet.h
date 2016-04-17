#ifndef I_PRIMARY_PACKET_H
#define I_PRIMARY_PACKET_H

#include "Sync_Packet.h"

struct primary_update {
	uint16_t id;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t target_x;
	uint32_t target_y;
	uint32_t target_z;
	uint16_t damage;
	uint8_t removed;
};

class I_Primary_Packet : public Sync_Packet {
	public:
		I_Primary_Packet(struct packet_header*, struct sync_header*, struct primary_update*);

                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct primary_update*);

                static size_t get_packet_size();

	private:
		const struct primary_update primary_data;

};

#endif
