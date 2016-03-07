# GNU Makefile for Linux Distros #
# Peter Forbes 3/7/2016 #

######## VARS ########

CC=g++
TEST_DEPS = Client.h Session.h server_utils.h
TEST_OBJS = Client.o Session.o server_utils.o
EXTRA_FLAGS = -Wall -Wpedantic -g

######## BUILD RULES ########

launcher : launcher.o $(TEST_OBJS)
	$(CC) launcher.o $(TEST_OBJS) -o launcher

launcher.o : launcher.cxx
	$(CC) -c launcher.cxx -o launcher.o $(EXTRA_FLAGS)

Session.o : Session.cxx Session.h Client.h server_utils.o
	$(CC) -c Session.cxx -o Session.o $(EXTRA_FLAGS)

Client.o : Client.cxx Client.h
	$(CC) -c Client.cxx -o Client.o $(EXTRA_FLAGS)

server_utils.o : server_utils.cxx server_utils.h
	$(CC) -c server_utils.cxx -o server_utils.o $(EXTRA_FLAGS)

######## ADDITIONAL RULES ########

clean:
	rm *.o launcher
