#include "G_Move_Request_Packet.h" 

G_Move_Request_Packet::G_Move_Request_Packet(struct packet_header* p_h, struct event_header* e_h, struct move_request* m_r) : 
	Event_Packet(p_h, e_h),
	move(*m_r) {

}

void G_Move_Request_Packet::serialize(unsigned char* data) const {

	// Insert necessary data into packet
        memcpy(data,     &(move.x), 4);
        memcpy(data + 4, &(move.y), 4);
        memcpy(data + 8, &(move.z), 4);

        Event_Packet::add_header(data + 12);

}

void G_Move_Request_Packet::deserialize(unsigned char* tail, struct move_request* content) {

        memcpy( &(content->z), tail - 4,  4);
        memcpy( &(content->y), tail - 8,  4);
        memcpy( &(content->x), tail - 12, 4);

}

size_t G_Move_Request_Packet::get_packet_size() {
	return 12 + Event_Packet::get_header_size(); 
}
