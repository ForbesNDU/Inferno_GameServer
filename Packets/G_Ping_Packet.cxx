#include "G_Ping_Packet.h"

G_Ping_Packet::G_Ping_Packet(struct packet_header* p_header, struct g_ping* ping_content) :
	Packet(p_header),
	ping_data(*ping_content) {

}

void G_Ping_Packet::serialize(unsigned char* data, size_t* data_size) const {
	
	// Determine packet size and allocate space
	*data_size = sizeof(struct packet_header) + sizeof(struct g_ping);
	data = (unsigned char*)malloc(*data_size);

	// Copy necessary data into packet
	memcpy(data,     &(ping_data.ping),    1);
	memcpy(data + 1, &(ping_data.x),       4);
	memcpy(data + 5, &(ping_data.y),       4);
	memcpy(data + 9, &(ping_data.issuer), 16);	
	
	add_header(data + 25);	

}

void G_Ping_Packet::deserialize(unsigned char* tail, struct g_ping* content) {
	
	memcpy( &(content->issuer), tail - 16, 16);
	memcpy( &(content->y),      tail - 20,  4);
	memcpy( &(content->x),      tail - 24,  4);
	memcpy( &(content->ping),   tail - 25,  1);

}

size_t G_Ping_Packet::get_packet_size() {
	return (sizeof(struct packet_header) + sizeof(struct g_ping));
}
