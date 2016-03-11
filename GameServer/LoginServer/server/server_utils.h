#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <arpa/inet.h>
#include <iostream>
#include <iomanip>
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/sha.h>
#include <sstream>
#include <string>
#include <string.h>
#include <sys/socket.h>

// Utility for getting IP as string
void* get_in_addr( struct sockaddr* );

// Creates a SHA-256 hash
std::string hash_SHA_256( std::string );

// Setup a UDP socket locally
bool setup_UDP_connection(const char*, const char*, int*, struct sockaddr*, socklen_t*);

// Creates a timestamp for log files
std::string timestamp();

#endif
