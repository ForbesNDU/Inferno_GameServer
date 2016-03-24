#include "G_Auto_Notify_Packet.h"

G_Auto_Notify_Packet::G_Auto_Notify_Packet(struct packet_header* p_header, struct auto_notify* conf_header) :
	Packet(p_header),
	auto_confirm(conf_header) {

}

void G_Auto_Notify_Packet::serialize(unsigned char* data, size_t* data_size) const {

	// Determine packet size and allocate space
        *data_size = sizeof(struct packet_header) + sizeof(struct g_ping);
        data = (unsigned char*)malloc(*data_size);

	// Copy necessary layer data into packet
        memcpy(data,     &(auto_confirm.target),     2);
        memcpy(data + 2, &(auto_confirm.experience), 2);
        memcpy(data + 4, &(auto_confirm.damage),     2);
        memcpy(data + 6, &(auto_confirm.conf),       1);

        add_header(data + 7);	

}



size_t G_Auto_Notify_Packet::get_packet_size() {
	return (13 + 7); 
}
