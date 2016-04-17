#include "I_Local_Player_Packet.h"

I_Local_Player_Packet::I_Local_Player_Packet(struct packet_header* p_h, struct sync_header* s_h, struct player_update* p_u) : 
	Sync_Packet(p_h, s_h),
	player_data(*p_u) {

}

void I_Local_Player_Packet::serialize(unsigned char* data) const {

        memcpy(data,      &(player_data.player_id),     2);
        memcpy(data + 2,  &(player_data.gold),          2);
        memcpy(data + 4,  &(player_data.health),        2);
        memcpy(data + 6,  &(player_data.max_health),    2);
        memcpy(data + 8,  &(player_data.mana),          2);
        memcpy(data + 10, &(player_data.max_mana),      2);
        memcpy(data + 12, &(player_data.cs),            2);
        memcpy(data + 14, &(player_data.experience),    4);
        memcpy(data + 18, &(player_data.x),             4);
        memcpy(data + 22, &(player_data.y),             4);
        memcpy(data + 26, &(player_data.z),             4);
        memcpy(data + 30, &(player_data.rot_x),         4);
        memcpy(data + 34, &(player_data.rot_y),         4);
        memcpy(data + 38, &(player_data.rot_z),         4); 
        memcpy(data + 42, &(player_data.auto_target),   2);
        memcpy(data + 44, &(player_data.respawn_timer), 1);
        memcpy(data + 45, &(player_data.level),         1);

	Sync_Packet::add_header(data + 46);

}

void I_Local_Player_Packet::deserialize(unsigned char* tail, struct player_update *content) {

        memcpy( &(content->level),         tail - 1,   1);
        memcpy( &(content->respawn_timer), tail - 2,   1);
        memcpy( &(content->auto_target),   tail - 4,   2);
        memcpy( &(content->rot_z),         tail - 8,   4);
        memcpy( &(content->rot_y),         tail - 12,  4);
        memcpy( &(content->rot_x),         tail - 16,  4);
        memcpy( &(content->z),             tail - 20,  4);
        memcpy( &(content->y),             tail - 24,  4);
        memcpy( &(content->x),             tail - 28,  4);
        memcpy( &(content->experience),    tail - 32,  4);
        memcpy( &(content->cs),            tail - 34,  2);
        memcpy( &(content->max_mana),      tail - 36,  2);
        memcpy( &(content->mana),          tail - 38,  2);
        memcpy( &(content->max_health),    tail - 40,  2);
        memcpy( &(content->health),        tail - 42,  2);
        memcpy( &(content->gold),          tail - 44,  2);
        memcpy( &(content->player_id),     tail - 46,  2);
       
}

size_t I_Local_Player_Packet::get_packet_size() {
	return 46 + Sync_Packet::get_header_size();	
}

















