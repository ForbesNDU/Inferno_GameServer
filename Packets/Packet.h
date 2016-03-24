#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define GAME_ID (59343074)

typedef enum packet_type : uint8_t {
	STATE_CHANGE_PACKET,
	STATE_SYNC_PACKET
} packet_type;

struct packet_header {
	uint32_t game_ID;
	uint16_t server_ID;
	uint16_t sequence_num;
	uint32_t ACK_bitfield;
	uint8_t type;
};

class Packet {
	public:
		Packet(struct packet_header*);

		virtual void serialize(unsigned char*, size_t*) const = 0;
		static void deserialize(unsigned char**, struct packet_header*);

	protected:
		// Utilities for subclasses to add general packet header
		void add_header(unsigned char*) const;

		// General header contents
		const struct packet_header header;

};

#endif
