#ifndef MATCHMAKER_H
#define MATCHMAKER_H

#include <arpa/inet.h>
#include <boost/lockfree/spsc_queue.hpp>
#include <deque>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <thread>
#include "server_utils.h"
#include <sys/socket.h>
#include <sys/time.h>

#define PORT "30000"

class MatchMaker {
	public:
		MatchMaker();
		~MatchMaker();

		void update_tokens();

	private:

		// Token Management
		void scrub_list();
		void receive_new();

		std::map<std::string, std::string>* current_users;
                std::deque<std::pair<std::string, long> >* expiry;

                struct sockaddr login_addr;
                socklen_t login_addrlen;

                int login_sock;
                FILE* logfile;

		// All Queue
		boost::lockfree::spsc_queue< boost::lockfree::capacity<500> > all_pending;

		// Rank-Based Queueing
		boost::lockfree::spsc_queue< boost::lockfree::capacity<150> > novice_queue;
		boost::lockfree::spsc_queue< boost::lockfree::capacity<200> > contender_queue;
                boost::lockfree::spsc_queue< boost::lockfree::capacity<200> > master_queue;
                boost::lockfree::spsc_queue< boost::lockfree::capacity<100> > punisher_queue;
                boost::lockfree::spsc_queue< boost::lockfree::capacity<50> > ultimate_queue;

		// Positional-Preference-Based Queueing
                boost::lockfree::spsc_queue< boost::lockfree::capacity<100> > magic_hybrid_queue;
		boost::lockfree::spsc_queue< boost::lockfree::capacity<100> > physical_hybrid_queue;
                boost::lockfree::spsc_queue< boost::lockfree::capacity<100> > support_queue;
                boost::lockfree::spsc_queue< boost::lockfree::capacity<100> > glass_cannon_queue;
                boost::lockfree::spsc_queue< boost::lockfree::capacity<100> > juggernaut_queue;	
                boost::lockfree::spsc_queue< boost::lockfree::capacity<200> > fill_queue;

};

#endif
