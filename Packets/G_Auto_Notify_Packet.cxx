#include "G_Auto_Notify_Packet.h"

G_Auto_Notify_Packet::G_Auto_Notify_Packet(struct packet_header* p_header, struct event_header* e_h, struct auto_notify* conf_header) :
	Event_Packet(p_header, e_h),
	auto_confirm(*conf_header) {

}

void G_Auto_Notify_Packet::serialize(unsigned char* data) const {

	// Copy necessary layer data into packet
        memcpy(data,     &(auto_confirm.target),     2);
        memcpy(data + 2, &(auto_confirm.experience), 2);
        memcpy(data + 4, &(auto_confirm.damage),     2);
        memcpy(data + 6, &(auto_confirm.conf),       1);

        Event_Packet::add_header(data + 7);	

}

void G_Auto_Notify_Packet::deserialize(unsigned char* tail, struct auto_notify* content) {

        memcpy( &(content->conf),       tail - 1, 1);
        memcpy( &(content->damage),     tail - 3, 2);
        memcpy( &(content->experience), tail - 5, 2);
        memcpy( &(content->target),     tail - 7, 2);

}

size_t G_Auto_Notify_Packet::get_packet_size() {
	return 7 + Event_Packet::get_header_size(); 
}
