#include "Socket.h"

////////// TOP LEVEL //////////

std::unordered_set<uint16_t> Socket::open_ports;

Socket::Socket(uint16_t req_port) : 
	remote_host("*"), 
	port( (boost::lexical_cast<std::string>((int)req_port)).c_str() ) {

	if( (open_ports.count(req_port) > 0) || (req_port < 1000) ) {
		throw PORT_EXCEPTION;
	} else {
		open_ports.insert(req_port);
	}

}

Socket::Socket(char* hostname, uint16_t req_port) : 
	remote_host(hostname),
	port( (boost::lexical_cast<std::string>((int)req_port)).c_str() ) {

        if( (open_ports.count(req_port) > 0) || (req_port < 1000) ) {
                throw PORT_EXCEPTION;
        } else {
                open_ports.insert(req_port);
        }	

}

////////// PUBLIC METHODS //////////

bool Socket::is_port_available(uint16_t req_port) {

	if(open_ports.count(req_port) > 0) {
		return false;
	} else {
		return true;
	}

}
