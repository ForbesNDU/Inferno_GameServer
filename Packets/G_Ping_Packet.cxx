#include "G_Ping_Packet.h"

G_Ping_Packet::G_Ping_Packet(struct packet_header* p_header, struct event_header* e_h, struct g_ping* ping_content) :
	Event_Packet(p_header, e_h),
	ping_data(*ping_content) {

}

void G_Ping_Packet::serialize(unsigned char* data) const {
	
	// Copy necessary data into packet
	memcpy(data,     &(ping_data.ping),    1);
	memcpy(data + 1, &(ping_data.x),       4);
	memcpy(data + 5, &(ping_data.y),       4);
	
	Event_Packet::add_header(data + 9);	

}

void G_Ping_Packet::deserialize(unsigned char* tail, struct g_ping* content) {
	
	memcpy( &(content->y),      tail - 4,  4);
	memcpy( &(content->x),      tail - 8,  4);
	memcpy( &(content->ping),   tail - 9,  1);

}

size_t G_Ping_Packet::get_packet_size() {
	return 9 + Event_Packet::get_header_size();
}
