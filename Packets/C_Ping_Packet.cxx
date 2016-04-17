#include "C_Ping_Packet.h"

C_Ping_Packet::C_Ping_Packet(struct packet_header* p_h, struct protocol_header* pr_h, struct c_ping* c_p) :
	Protocol_Packet(p_h, pr_h),
	rtt(*c_p) {

}

void C_Ping_Packet::serialize(unsigned char* data) const {	

	// Copy necessary data into packet
        memcpy(data, &(rtt.stamp), 1);

        Protocol_Packet::add_header(data + 1);

}

void C_Ping_Packet::deserialize(unsigned char* tail, struct c_ping* content) {

        memcpy( &(content->stamp), tail - 1, 1);

}

size_t C_Ping_Packet::get_packet_size() {
	return 1 + Protocol_Packet::get_header_size(); 
}
