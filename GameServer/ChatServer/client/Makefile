# GNU Makefile for Linux Distros #
# Peter Forbes 3/7/2016 #

######## VARS ########

CC=g++
TEST_DEPS = server_utils.h
TEST_OBJS = server_utils.o
EXTRA_FLAGS = -Wall -Wpedantic -g

######## BUILD RULES ########

client : client.o $(TEST_OBJS)
	$(CC) client.o $(TEST_OBJS) -o client

client.o : client.cxx
	$(CC) -c client.cxx -o client.o $(EXTRA_FLAGS)

server_utils.o : server_utils.cxx server_utils.h
	$(CC) -c server_utils.cxx -o server_utils.o $(EXTRA_FLAGS)

######## ADDITIONAL RULES ########

clean:
	rm *.o client

