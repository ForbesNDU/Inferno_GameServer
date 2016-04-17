#ifndef EVENT_PACKET_H
#define EVENT_PACKET_H

#include "Packet.h"

typedef enum event_type : uint8_t {
	AUTO_REQUEST,
	AUTO_NOTIFY
} event_type;

struct event_header {
	uint8_t priority;
	event_type event;
};

class Event_Packet : public Packet {
	public:
		Event_Packet(struct packet_header*, struct event_header*);

                virtual void serialize(unsigned char*) const = 0;
                static void deserialize(unsigned char**, struct event_header*);

	protected:
                // Utilities for subclasses to add general event packet header
                void add_header(unsigned char*) const;
		static size_t get_header_size();               
 
                // General header contents
		const struct event_header e_header;

};

#endif
