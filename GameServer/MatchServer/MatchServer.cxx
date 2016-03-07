#include "MatchServer.h"

////////// PUBLIC //////////

MatchServer::MatchServer() {

}

MatchServer::~MatchServer() {

}

bool MatchServer::addClient(Client* newClient) {
	
	if(clients.size() == CLIENTSPERSERVER) {
		return false;
	} else {
		clients.push_back(newClient);	
		recalculateELO(newClient->getELO());
		return true;	
	}

}

int MatchServer::getClientCount() {
	return clients.size();
}

int MatchServer::getELO() {
	return serverELO;
}

////////// PRIVATE //////////

void MatchServer::recalculateELO(int newValue) {

	if(clients.size() == 1) {

		serverELO = newValue;

	} else {

		double addition = (newValue - serverELO) / clients.size();
		serverELO += (int)addition;		

	}

}

