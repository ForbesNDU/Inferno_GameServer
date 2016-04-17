#include "I_Auto_Packet.h"

I_Auto_Packet::I_Auto_Packet(struct packet_header* p_h, struct sync_header* s_h, struct auto_update* a_u) : 
	Sync_Packet(p_h, s_h),
	auto_data(*a_u) {

}

void I_Auto_Packet::serialize(unsigned char* data) const {

	// Copy necessary data into packet
        memcpy(data,      &(auto_data.x),       4);
        memcpy(data + 4,  &(auto_data.y),       4);
        memcpy(data + 8,  &(auto_data.z),       4);
	memcpy(data + 12, &(auto_data.owner),   2);
        memcpy(data + 14, &(auto_data.target),  2);
        memcpy(data + 16, &(auto_data.damage),  2);	
	memcpy(data + 18, &(auto_data.removed), 1);

        Sync_Packet::add_header(data + 19);

}

void I_Auto_Packet::deserialize(unsigned char* tail, struct auto_update* content) {

        memcpy( &(content->removed), tail - 1,  1);
        memcpy( &(content->damage),  tail - 3,  2);
        memcpy( &(content->target),  tail - 5,  2);
        memcpy( &(content->owner),   tail - 7,  2);
        memcpy( &(content->z),       tail - 11, 4);
        memcpy( &(content->y),       tail - 15, 4);
        memcpy( &(content->x),       tail - 19, 4);

}

size_t I_Auto_Packet::get_packet_size() {
	return 19 + Sync_Packet::get_header_size();
}
