#ifndef I_OBJECTIVE_PACKET_H
#define I_OBJECTIVE_PACKET_H

#include "Packet.h"

struct objective_update {
	uint16_t objective_id;
	uint16_t health;
	uint16_t target_id;
	uint8_t is_active;
};

class I_Objective_Packet : public I_Objective_Packet {
	public:
		I_Objective_Packet(struct packet_header*, struct objective_update*);		

                void serialize(unsigned char*, size_t*) const;
                static void deserialize(unsigned char*, struct objective_update*);

                static size_t get_packet_size();

	private:
		const struct objective_update obj_data;

};

#endif
