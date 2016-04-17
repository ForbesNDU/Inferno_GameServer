#include "Sync_Packet.h"

Sync_Packet::Sync_Packet(struct packet_header* p_header, struct sync_header* sy_header) :
	Packet(p_header),
	s_header(*sy_header) {

}

void Sync_Packet::deserialize(unsigned char** tail, struct sync_header* header) {

	// Strip header info (from packet tail)
        memcpy( &(header->priority), *tail - 2, 1 );
        memcpy( &(header->sync),     *tail - 1, 1 );

        *tail = *tail - 2;

}

void Sync_Packet::add_header(unsigned char* tail) const {

	// Add header info (on packet tail)
        memcpy(tail,     &s_header.priority, 1);
        memcpy(tail + 1, &s_header.sync,     1);

	Packet::add_header(tail);

}
