#include "MatchMaker.h"

MatchMaker::MatchMaker() {
	toKill = false;
}

MatchMaker::~MatchMaker() {

}

void MatchMaker::acceptor() {

	// Setup TCP connection
	

	while(!toKill) {

	}

}

void MatchMaker::dispatcher() {

	while(!toKill) {

	}

}

void MatchMaker::launch() {

	std::thread t0{this->acceptor}
	std::thread t1{this->dispatcher}
	t0.join();
	t1.join();

}
