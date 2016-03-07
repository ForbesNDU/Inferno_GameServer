#ifndef MATCHSERVER_H
#define MATCHSERVER_H

#include "Client.h"
#include "netdefs.h"
#include <vector>

class MatchServer {
	public:
		MatchServer();
		~MatchServer();
		bool addClient(Client*);
		int getClientCount();
		int getELO();

	private:
		void recalculateELO(int);

		std::vector<Client*> clients;
		int serverELO;

};

#endif
