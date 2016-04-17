#ifndef I_LASTHIT_PACKET_H
#define I_LASTHIT_PACKET_H

#include "Sync_Packet.h"

typedef enum kill_type : uint8_t {
	MINION_KILL,
	PLAYER_KILL,
	TOWER_KILL,
	SHIELD_GENERATOR_KILL,
	CORE_KILL
} kill_type;

struct lasthit_update {
	uint16_t attacker_id;
	uint16_t target_id;
	kill_type kill;
};

class I_LastHit_Packet : public Sync_Packet {
	public:
		I_LastHit_Packet(struct packet_header*, struct sync_header*, struct lasthit_update*);

                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct lasthit_update*);

                static size_t get_packet_size();

	private:
		const struct lasthit_update lh_data;

};

#endif
