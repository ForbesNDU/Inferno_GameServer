#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void* get_in_addr(struct sockaddr*);

bool setup_TCP_connection(const char*, const char*);

bool setup_UDP_connection(const char*, const char*, int*, struct sockaddr*, socklen_t*);

std::string timestamp();

#endif
