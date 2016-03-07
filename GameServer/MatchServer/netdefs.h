// Defines top level modules for the game server networking protocol //
#ifndef NETDEFS_H
#define NETDEFS_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAXPACKETSIZE (1024)
#define CLIENTSPERSERVER (10)

////////// ENUMERATIONS //////////

enum clientAction : uint8_t {

        MOVE_TO,
        Q_ATTACK,
        W_ATTACK,
        E_ATTACK,
        R_ATTACK,
        PING_WARN,
        PING_MOVETO,
        PING_ASSIST,
        MATCH_REQ,
        QUIT

};

enum ranking : uint8_t {

	NOVICE,		// 20%
	CONTENDER,	// 30%
	MASTER,		// 30%
	PUNISHER,	// 15%
	ULTIMATE	// 5%

};

////////// STRUCTS //////////

struct packet {

	struct sockaddr_storage address;
	unsigned char data[MAXPACKETSIZE];

};

struct location_vector {

	double x;
	double y;
	double z;

};

struct direction {

	double rads;

};

struct ELO {

	int ELO;
	int MMR;
	ranking rank; 

};

#endif
