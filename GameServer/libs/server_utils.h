#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <sys/socket.h>

void* get_in_addr(struct sockaddr*);
int initialize_tcp_connection();
std::string timestamp();

#endif
