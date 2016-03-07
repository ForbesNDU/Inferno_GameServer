#include "Acceptor.h"
#include <arpa/inet.h>
#include <boost/lockfree/spsc_queue.hpp>
#include "Converter.h"
#include "netdefs.h"
#include <stdlib.h>
#include <thread>

int main(int argc, char **argv) {

        boost::lockfree::spsc_queue<struct packet, boost::lockfree::capacity<2000> > converter_queue;

	Acceptor acceptor(13000);
	Converter converter(&converter_queue);

	acceptor.attachConverterQueue(&converter_queue);
	acceptor.setup();

	std::thread acceptorLoop{acceptor.startReceiving};
	std::thread converterLoop{converter.convert};
	
	acceptorLoop.join();
	converterLoop.join();
	
	return EXIT_SUCCESS;

}
