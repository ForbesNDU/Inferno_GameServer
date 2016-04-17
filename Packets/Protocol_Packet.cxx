#include "Protocol_Packet.h"

Protocol_Packet::Protocol_Packet(struct packet_header* p_header, struct protocol_header* pro_header) :
	Packet(p_header),
	pr_header(*pro_header) {

}

void Protocol_Packet::deserialize(unsigned char** tail, struct protocol_header* header) {

	// Strip header info (from packet tail)
        memcpy( &(header->op), *tail - 1, 1 );

        *tail = *tail - 1;

}

void Protocol_Packet::add_header(unsigned char* tail) const {

	// Add header info (to packet tail)
        memcpy(tail, &pr_header.op, 1);

	Packet::add_header(tail);

}

size_t Protocol_Packet::get_header_size() {
	return 1 + Packet::get_header_size();
}
