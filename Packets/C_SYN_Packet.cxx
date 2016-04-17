#include "C_SYN_Packet.h"

C_SYN_Packet::C_SYN_Packet(struct packet_header* p_h, struct protocol_header* pr_h, struct syn_request* s_h) :
	Protocol_Packet(p_h, pr_h),
	syn_content(*s_h) {

}

void C_SYN_Packet::serialize(unsigned char* data) const {

	// Copy necessary data into packet
        memcpy(data,     &(syn_content.req),    1);
        memcpy(data + 1, &(syn_content.conf_number), 1);

        Protocol_Packet::add_header(data + 2);	

}

void C_SYN_Packet::deserialize(unsigned char* tail, struct syn_request* content) {

        memcpy( &(content->conf_number), tail - 1, 1);
        memcpy( &(content->req),         tail - 2, 1);

}

size_t C_SYN_Packet::get_packet_size() {
	return 2 + Protocol_Packet::get_header_size();
}
