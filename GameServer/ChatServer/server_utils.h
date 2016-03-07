#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <sys/socket.h>

void* get_in_addr(struct sockaddr*);
std::string timestamp();

#endif
