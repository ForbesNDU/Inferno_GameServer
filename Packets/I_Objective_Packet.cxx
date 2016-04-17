#include "I_Objective_Packet.h"

I_Objective_Packet::I_Objective_Packet(struct packet_header* p_h, struct sync_header* s_h, struct objective_update* o_u) : 
	Sync_Packet(p_h, s_h), 
	obj_data(*o_u) {

}

void I_Objective_Packet::serialize(unsigned char* data) const {

	// Copy necessary data into packet
        memcpy(data,      &(obj_data.id),          2);
        memcpy(data + 2,  &(obj_data.health),      2);
        memcpy(data + 4,  &(obj_data.max_health),  2);
        memcpy(data + 6,  &(obj_data.x),           4);
        memcpy(data + 10, &(obj_data.y),           4);
        memcpy(data + 14, &(obj_data.z),           4);
        memcpy(data + 18, &(obj_data.auto_target), 2);

	Sync_Packet::add_header(data + 20);

}

void I_Objective_Packet::deserialize(unsigned char* tail, struct objective_update* content) {

        memcpy( &(content->auto_target), tail - 2,  2);
        memcpy( &(content->z),           tail - 6,  4);
        memcpy( &(content->y),           tail - 10, 4);
        memcpy( &(content->x),           tail - 14, 4);
        memcpy( &(content->max_health),  tail - 16, 2);
        memcpy( &(content->health),      tail - 18, 2);
        memcpy( &(content->id),          tail - 20, 2);

}

size_t I_Objective_Packet::get_packet_size() {
	return 20 + Sync_Packet::get_header_size();
}
