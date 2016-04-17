#include "G_Primary_Request_Packet.h"

G_Primary_Request_Packet::G_Primary_Request_Packet(struct packet_header* p_h, struct event_header* e_h, struct primary_request* p_r) : 
	Event_Packet(p_h, e_h),
	attack(*p_r) {

}

void G_Primary_Request_Packet::serialize(unsigned char* data) const {

	// Copy necessary data into packet
        memcpy(data,      &(attack.p_attack), 1);
        memcpy(data + 1,  &(attack.player_x), 4);
        memcpy(data + 5,  &(attack.player_y), 4);
        memcpy(data + 9,  &(attack.player_z), 4);
        memcpy(data + 13, &(attack.target_x), 4);
        memcpy(data + 17, &(attack.target_y), 4);
        memcpy(data + 21, &(attack.target_z), 4);

        Event_Packet::add_header(data + 25);

}

void G_Primary_Request_Packet::deserialize(unsigned char* tail, struct primary_request* content) {

        memcpy( &(content->target_z), tail - 4,  4);
        memcpy( &(content->target_y), tail - 8,  4);
        memcpy( &(content->target_x), tail - 12, 4);
        memcpy( &(content->player_z), tail - 16, 4);
        memcpy( &(content->player_y), tail - 20, 4);
        memcpy( &(content->player_x), tail - 24, 4);
        memcpy( &(content->p_attack), tail - 25, 1);

}

size_t G_Primary_Request_Packet::get_packet_size() {
	return 25 + Event_Packet::get_header_size();
}
