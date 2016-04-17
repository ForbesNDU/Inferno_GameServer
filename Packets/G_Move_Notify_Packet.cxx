#include "G_Move_Notify_Packet.h"

G_Move_Notify_Packet::G_Move_Notify_Packet(struct packet_header* p_h, struct event_header* e_h, struct move_notify* m_n) :
	Event_Packet(p_h, e_h),
	move(*m_n) {

}

void G_Move_Notify_Packet::serialize(unsigned char* data) const {

	// Insert packet data
        memcpy(data,      &(move.x),    4);
        memcpy(data + 4,  &(move.y),    4);
        memcpy(data + 8,  &(move.z),    4);
        memcpy(data + 12, &(move.conf), 1);

        Event_Packet::add_header(data + 13);

}

void G_Move_Notify_Packet::deserialize(unsigned char* tail, struct move_notify* content) {

        memcpy( &(content->conf), tail - 1,  1);
        memcpy( &(content->z),    tail - 5,  4);
        memcpy( &(content->y),    tail - 9,  4);
        memcpy( &(content->x),    tail - 13, 4);

}

size_t G_Move_Notify_Packet::get_packet_size() {
	return 13 + Event_Packet::get_header_size();
}
