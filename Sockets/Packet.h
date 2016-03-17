#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

class Packet {
	public:
		Packet(uint16_t, uint32_t, unsigned char*);
		
		virtual unsigned char* serialize() const = 0;
		virtual static bool deserialize(Packet*) = 0;

	private:
		

};

#endif
