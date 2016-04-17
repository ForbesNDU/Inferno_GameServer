#include "I_LastHit_Packet.h"

I_LastHit_Packet::I_LastHit_Packet(struct packet_header* p_h, struct sync_header* s_h, struct lasthit_update* lhu) :
	Sync_Packet(p_h, s_h),
	lh_data(*lhu) {

}

void I_LastHit_Packet::serialize(unsigned char* data) const {

	// Copy data into packet
        memcpy(data,      &(lh_data.attacker_id), 2);
        memcpy(data + 2,  &(lh_data.target_id),   2);
        memcpy(data + 4,  &(lh_data.kill),        1);
        
	Sync_Packet::add_header(data + 5);

}

void I_LastHit_Packet::deserialize(unsigned char* tail, struct lasthit_update *content) {

        memcpy( &(content->kill),        tail - 1,  1);
        memcpy( &(content->target_id),   tail - 3,  2);
        memcpy( &(content->attacker_id), tail - 5,  2);

}

size_t I_LastHit_Packet::get_packet_size() {
	return 5 + Sync_Packet::get_header_size(); 
}
