#include "G_Auto_Request_Packet.h"

G_Auto_Request_Packet::G_Auto_Request_Packet(struct packet_header* p_h, struct event_header* e_h, struct auto_request* a_r) : 
	Event_Packet(p_h, e_h),
	auto_attack(*a_r) {

}

void G_Auto_Request_Packet::serialize(unsigned char* data) const {

	// Copy necessary data into packet
        memcpy(data, &(auto_attack.target), 2);

        Event_Packet::add_header(data + 2);

}

void G_Auto_Request_Packet::deserialize(unsigned char* tail, struct auto_request* content) {
        memcpy( &(content->target), tail - 2, 2);
}

size_t G_Auto_Request_Packet::get_packet_size() {
	return 2 + Event_Packet::get_header_size();
}
