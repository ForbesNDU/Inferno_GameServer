#include "Client.h"

Client::Client(int newELO) {
	ELO = newELO;
}

Client::~Client() {

}

int Client::getELO() {
	return ELO;
}
