#ifndef CONVERTER_H
#define CONVERTER_H

#include <boost/lockfree/spsc_queue.hpp>
#include <unordered_map>
#include "MatchServer.h"
#include "netdefs.h"
#include <stdint.h>

class Converter {
	public:
		Converter(boost::lockfree::spsc_queue<struct packet, boost::lockfree::capacity<2000> >*);
		~Converter();		

		void convert();
		void attachMatchServer(MatchServer&);

	private:
		void processPacket(struct packet*);

                boost::lockfree::spsc_queue<struct packet, boost::lockfree::capacity<2000> > *fromAcceptor;		
		MatchServer* activeMatches;

};

#endif
