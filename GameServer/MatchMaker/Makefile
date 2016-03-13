# GNU Makefile for Linux Distros #
# Peter Forbes 3/12/2016 #

######## VARS ########

CC=g++
TEST_DEPS = MatchMaker.h Client.h Session.h server_utils.h
TEST_OBJS = MatchMaker.o Client.o Session.o server_utils.o
EXTRA_FLAGS = -Wall -Wpedantic -g -std=c++11

######## BUILD RULES ########

launcher : launcher.o $(TEST_OBJS)
	$(CC) launcher.o $(TEST_OBJS) -o launcher

launcher.o : launcher.cxx
	$(CC) -c launcher.cxx -o launcher.o $(EXTRA_FLAGS)

MatchMaker.o : MatchMaker.cxx MatchMaker.h server_utils.o
	$(CC) -c MatchMaker.cxx -o MatchMaker.o $(EXTRA_FLAGS)

server_utils.o : server_utils.cxx server_utils.h
	$(CC) -c server_utils.cxx -o server_utils.o $(EXTRA_FLAGS)

######## ADDITIONAL RULES ########

clean:
	rm *.o launcher
