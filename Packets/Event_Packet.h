#ifndef EVENT_PACKET_H
#define EVENT_PACKET_H

#include "Packet.h"

typedef event_header enum : uint8_t {
	AUTO_REQUEST,
	AUTO_NOTIFY
} event_header;

struct event_header {
	uint8_t priority;
	event_type event;
};

class Event_Packet : public Packet {
	public:
		Event_Packet(struct packet_header*, struct event_header*);

                virtual void serialize(unsigned char*, size_t*) const = 0;
                static void deserialize(unsigned char**, struct event_header*);

	protected:
                // Utilities for subclasses to add general event packet header
                void add_header(unsigned char*) const;
                
                // General header contents
		const struct event_header* e_header;

};

#endif
