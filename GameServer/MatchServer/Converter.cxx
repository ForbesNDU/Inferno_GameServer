#include "Converter.h"

Converter::Converter(boost::lockfree::spsc_queue<struct packet, boost::lockfree::capacity<2000> >* attachedQueue) {
	fromAcceptor = attachedQueue;
}

Converter::~Converter() {

}

void Converter::convert() {

	if(!fromAcceptor->is_lock_free()) {
		std::cout << "Warning: input processing queue is not lock-free" << std::endl;
	}

	struct packet currentPacket;

	while(1) {
		
		fromAcceptor->pop(currentPacket);
		processPacket(&currentPacket);

	}

}

void Converter::processPacket(struct packet* currentPacket) {

	// Read request code
	uint8_t code; 
	memcpy(&code, currentPacket->data, sizeof(uint8_t));

}
