#include "Event_Packet.h"

Event_Packet::Event_Packet(struct packet_header* p_header, event_header* ev_header) : 
	Packet(p_header),
	e_header(*ev_header) {

}

void Event_Packet::deserialize(unsigned char** tail, struct event_header* header) {

	// Strip header info (from packet tail)
        memcpy( &(header->priority), *tail - 2, 1 );
        memcpy( &(header->event),    *tail - 1, 1 );

        *tail = *tail - 2;	

}

void Event_Packet::add_header(unsigned char* tail) const {

	// Add header info (on packet tail)
        memcpy(tail,     &e_header.priority, 1);
        memcpy(tail + 1, &e_header.event,    1);

	Packet::add_header(tail);

}

size_t Event_Packet::get_header_size() {
	return 2 + Packet::get_header_size();
}	
