#include "G_Primary_Notify_Packet.h" 

G_Primary_Notify_Packet::G_Primary_Notify_Packet(struct packet_header* p_h, struct event_header* e_h, struct primary_notify* p_n) :
	Event_Packet(p_h, e_h),
	attack(*p_n) {

}

void G_Primary_Notify_Packet::serialize(unsigned char* data) const  {

	// Copy data into packet
        memcpy(data,      &(attack.p_attack), 1);
        memcpy(data + 1,  &(attack.player_x), 4);
        memcpy(data + 5,  &(attack.player_y), 4);
	memcpy(data + 9,  &(attack.player_z), 4);
	memcpy(data + 13, &(attack.target_x), 4);
	memcpy(data + 17, &(attack.target_y), 4);
	memcpy(data + 21, &(attack.target_z), 4);
	memcpy(data + 25, &(attack.conf),     1);	

        Event_Packet::add_header(data + 26);

}

void G_Primary_Notify_Packet::deserialize(unsigned char* tail, struct primary_notify* content) {

        memcpy( &(content->conf),     tail - 1,  1);
        memcpy( &(content->target_z), tail - 5,  4);
        memcpy( &(content->target_y), tail - 9,  4);
        memcpy( &(content->target_x), tail - 13, 4);
        memcpy( &(content->player_z), tail - 17, 4);
        memcpy( &(content->player_y), tail - 21, 4);
        memcpy( &(content->player_x), tail - 25, 4);
        memcpy( &(content->p_attack), tail - 26, 1);

}

size_t G_Primary_Notify_Packet::get_packet_size() {
	return 26 + Event_Packet::get_header_size();
}
