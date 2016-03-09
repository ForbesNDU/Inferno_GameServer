#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <sys/socket.h>

// Utility for getting IP as string
void* get_in_addr( struct sockaddr* );

// Creates a SHA-256 hash
std::string hash_SHA_256( std::string );

// Creates a timestamp for log files
std::string timestamp();

#endif
