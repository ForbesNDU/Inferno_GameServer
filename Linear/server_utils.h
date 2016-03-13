#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <arpa/inet.h>
#include <iostream>
#include <iomanip>
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/sha.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void* get_in_addr(struct sockaddr*);

std::string hash_SHA_256(std::string);

bool setup_TCP_connection(const char*, const char*, int*);

bool setup_UDP_client_connection(const char*, const char*, int*, struct sockaddr*, socklen_t*);

bool setup_UDP_server_connection(const char*, int*);

std::string timestamp();

#endif
