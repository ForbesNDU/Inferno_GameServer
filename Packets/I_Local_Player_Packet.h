#ifndef I_LOCAL_PLAYER_PACKET_H
#define I_LOCAL_PLAYER_PACKET_H

#include "Sync_Packet.h"

struct player_update {
	uint16_t player_id;
	uint16_t gold;
	uint16_t health;
	uint16_t max_health;
	uint16_t mana;
	uint16_t max_mana;
	uint16_t cs;
	uint32_t experience;
	uint32_t x;
	uint32_t y;
	uint32_t z;	
	uint32_t rot_x;
	uint32_t rot_y;
	uint32_t rot_z;
	uint16_t auto_target;
	uint8_t respawn_timer;
	uint8_t level;
};

class I_Local_Player_Packet : public Sync_Packet {
	public:
		I_Local_Player_Packet(struct packet_header*, struct sync_header*, struct player_update*);

                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct player_update*);

                static size_t get_packet_size();	

	private:
		const struct player_update player_data;

};

#endif
