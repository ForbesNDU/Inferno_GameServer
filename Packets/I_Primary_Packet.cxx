#include "I_Primary_Packet.h"

I_Primary_Packet::I_Primary_Packet(struct packet_header* p_h, struct sync_header* s_h, struct primary_update* p_u) :
	Sync_Packet(p_h, s_h),
	primary_data(p_u) {

}

void I_Primary_Packet::serialize(unsigned char* data) const {

	// Copy necessary data into packet
        memcpy(data,      &(primary_data.id),       2);
        memcpy(data + 2,  &(primary_data.x),        4);
        memcpy(data + 6,  &(primary_data.y),        4);
        memcpy(data + 10, &(primary_data.z),        4);
        memcpy(data + 14, &(primary_data.target_x), 4);
        memcpy(data + 18, &(primary_data.target_y), 4);
        memcpy(data + 22, &(primary_data.target_z), 4);
        memcpy(data + 26, &(primary_data.damage),   2);
        memcpy(data + 28, &(primary_data.removed),  1);

	Sync_Packet::add_header(data + 29);

}

void I_Primary_Packet::deserialize(unsigned char* tail, struct primary_update* content) {

        memcpy( &(content->removed),  tail - 1,  1);
        memcpy( &(content->damage),   tail - 3,  2);
        memcpy( &(content->target_z), tail - 7,  4);
        memcpy( &(content->target_y), tail - 11, 4);
        memcpy( &(content->target_x), tail - 15, 4);
        memcpy( &(content->z),        tail - 19, 4);
        memcpy( &(content->y),        tail - 23, 4);
        memcpy( &(content->x),        tail - 27, 4);
        memcpy( &(content->id),       tail - 29, 2);

}

size_t I_Primary_Packet::get_packet_size() {
	return 29 + Sync_Packet::get_header_size();	
}
