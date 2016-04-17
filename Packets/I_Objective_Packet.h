#ifndef I_OBJECTIVE_PACKET_H
#define I_OBJECTIVE_PACKET_H

#include "Sync_Packet.h"

struct objective_update {
	uint16_t id;
	uint16_t health;
	uint16_t max_health;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint16_t auto_target;
};

class I_Objective_Packet : public Sync_Packet {
	public:
		I_Objective_Packet(struct packet_header*, struct sync_header*, struct objective_update*);		

                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct objective_update*);

                static size_t get_packet_size();

	private:
		const struct objective_update obj_data;

};

#endif
