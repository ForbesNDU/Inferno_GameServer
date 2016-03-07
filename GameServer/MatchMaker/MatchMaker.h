#ifndef MATCHMAKER_H
#define MATCHMAKER_H

#include <boost/lockfree/spsc_queue.hpp>
#include <thread>

class MatchMaker {
	public:
		MatchMaker();
		~MatchMaker();

		void acceptor();
		void matchmake();

		void launch();

	private:

		// Control
		bool toKill;

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
