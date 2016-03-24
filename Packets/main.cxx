#include "G_Ping_Packet.h"
#include <iostream>
#include "stdlib.h"
#include "string.h"

int main(int argc, char** argv) {
	
	struct packet_header header {
		GAME_ID,
		0,
		0,
		0,
		STATE_CHANGE_PACKET
	};

	struct g_ping ping {
		PING_ASSIST,
		0,
		0,
		"Aix la Chapelle"
	};

	// Serialize
	G_Ping_Packet* ping_packet = new G_Ping_Packet(&header, &ping);

	unsigned char* data = (unsigned char*)malloc( G_Ping_Packet::get_packet_size() );
	memset(data, 0, G_Ping_Packet::get_packet_size());
	size_t size;
	ping_packet->serialize(data, &size);		

	return EXIT_SUCCESS;

}
