#ifndef G_PING_PACKET_H
#define G_PING_PACKET_H

#include "Event_Packet.h"

// Ping Types //
typedef enum ping_type : uint8_t {
	PING_WARNING,
	PING_ASSIST,
	PING_TARGET,
	PING_MOVETO
} ping_type;

// Data needed to represent the ping accurately on a remote client //
struct g_ping {
	ping_type ping;
	uint32_t x;
	uint32_t y;
};

// Packet carrying the ping data //
class G_Ping_Packet : public Event_Packet {
	public:
		G_Ping_Packet(struct packet_header*, struct event_header*, struct g_ping*);

		void serialize(unsigned char*) const;
		static void deserialize(unsigned char*, struct g_ping*);

		static size_t get_packet_size();

	private:
		const struct g_ping ping_data;

};

#endif
