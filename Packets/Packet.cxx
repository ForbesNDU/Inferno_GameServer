#include "Packet.h"

Packet::Packet(struct packet_header* p_header) : header(*p_header) {

}

void Packet::deserialize(unsigned char** tail, struct packet_header* p_header) {

        // Strip header info (on packet tail)
	memcpy( &(p_header->game_ID),      *tail - 12, 4 );
        memcpy( &(p_header->server_ID),    *tail - 8,  2 );
        memcpy( &(p_header->sequence_num), *tail - 6,  2 );
        memcpy( &(p_header->ACK_bitfield), *tail - 4,  4 );

        *tail = *tail - 12;


}

void Packet::add_header(unsigned char* tail) const {

	// Add header info (on packet tail)
	memcpy(tail,     &header.game_ID,      4);
	memcpy(tail + 4, &header.server_ID,    2);
	memcpy(tail + 6, &header.sequence_num, 2);
	memcpy(tail + 8, &header.ACK_bitfield, 4);

}

