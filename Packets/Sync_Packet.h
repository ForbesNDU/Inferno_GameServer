#ifndef SYNC_PACKET_H
#define SYNC_PACKET_H

#include "Packet.h"

typedef enum sync_type : uint8_t {
	LASTHIT_UPDATE,
	OBJECTIVE_UPDATE,
	LOCAL_PLAYER_UPDATE,
	REMOTE_PLAYER_UPDATE	
} sync_type;

struct sync_header {
	uint8_t priority;
	sync_type sync;
};

class Sync_Packet : public Packet {
	public:
		Sync_Packet(struct packet_header*, struct sync_header*);

                virtual void serialize(unsigned char*) const = 0;
                static void deserialize(unsigned char**, struct sync_header*);

	protected:
                // Utilities for subclasses to add general sync packet header
                void add_header(unsigned char*) const;
                
                // General header contents
		const struct sync_header s_header;

};

#endif
