# Peter Forbes 2/4/2016 #
# LINUX MATCH SERVER    #

######## VARS ########

CC = g++
TEST_DEPS = Acceptor.h
TEST_OBJS = Acceptor.o
EXTRA_FLAGS = -std=c++11 -Wall -Wextra -pedantic -g

######## BUILD RULES ########

launcher : launcher.o $(TEST_OBJS)
	$(CC) launcher.o $(TEST_OBJS) -o launcher
	
launcher.o : launcher.cxx
	$(CC) -c launcher.cxx -o launcher.o $(EXTRA_FLAGS)

Acceptor.o : Acceptor.cxx Acceptor.h netdefs.h 
	$(CC) -c Acceptor.cxx -o Acceptor.o $(EXTRA_FLAGS)

Converter.o : Converter.cxx Converter.h netdefs.h
	$(CC)  -c Converter.cxx -o Converter.o $(EXTRA_FLAGS)

MatchServer.o : MatchServer.cxx MatchServer.h
	$(CC) -c MatchServer.cxx -o MatchServer.o $(EXTRA_FLAGS)

Client.o : Client.cxx Client.h
	$(CC) -c Client.cxx -o Client.o $(EXTRA_FLAGS)

######## ADD'L RULES ########

clean:
	rm *.o launcher
