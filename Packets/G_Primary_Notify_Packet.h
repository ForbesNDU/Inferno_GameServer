#ifndef G_PRIMARY_NOTIFY_PACKET_H
#define G_PRIMARY_NOTIFY_PACKET_H

#include "Packet.h"
#include "G_Primary_Defs.h"

struct primary_notify {
	attack_type attack;
        uint32_t player_x;
        uint32_t player_y;
        uint32_t player_z;
        uint32_t target_x;
        uint32_t target_y;
        uint32_t target_z;
	uint16_t experience;
	uint8_t conf;	
};

class G_Primary_Notify_Packet : public Packet {
	public:
		G_Primary_Notify_Packet(struct packet_header*, struct primary_notify*);

                void serialize(unsigned char*, size_t*) const;
                static void deserialize(unsigned char*, struct primary_notify*);

                static size_t get_packet_size();

	private:	
		const struct primary_notify attack;

};

#endif
