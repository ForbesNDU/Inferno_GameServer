#ifndef G_PRIMARY_REQUEST_PACKET
#define G_PRIMARY_REQUEST_PACKET

#include "Event_Packet.h"
#include "G_Primary_Defs.h"

struct primary_request {
	attack_type p_attack;
	uint32_t player_x;
	uint32_t player_y;
	uint32_t player_z;
	uint32_t target_x;
	uint32_t target_y;
	uint32_t target_z;
};

class G_Primary_Request_Packet : public Event_Packet {
	public:
		G_Primary_Request_Packet(struct packet_header*, struct event_header*, struct primary_request*);

                void serialize(unsigned char*) const;
                static void deserialize(unsigned char*, struct primary_request*);

                static size_t get_packet_size();

	private:
		const struct primary_request attack;

};

#endif 
