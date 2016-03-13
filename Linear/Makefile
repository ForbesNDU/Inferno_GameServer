# GNU Makefile for Linux Server
# Peter Forbes 1/30/2016


######## VARS ########


CC = g++
LAUNCHER_DEPS = LoginServer.h ChatServer.h MatchMaker.h server_utils.h
LAUNCHER_OBJS = LoginServer.o ChatServer.o MatchMaker.o server_utils.o
EXTRA_FLAGS = -Wall -Wpedantic -g -std=c++11
CRYPTO_FLAGS = -lssl -lcrypto
MYSQL_FLAGS =  -lmysqlcppconn
LIBS = -L/usr/lib 


######## LAUNCHER BUILD RULES ########


launcher : launcher.o $(LAUNCHER_OBJS)
	$(CC) launcher.o $(LAUNCHER_OBJS) -o launcher $(CRYPTO_FLAGS) $(MYSQL_FLAGS)

launcher.o : launcher.cxx
	$(CC) -c launcher.cxx -o launcher.o $(EXTRA_FLAGS)


######## SERVER COMPONENT BUILD RULES ########


LoginServer.o : LoginServer.cxx LoginServer.h server_utils.h
	$(CC) $(EXTRA_FLAGS) -c LoginServer.cxx $(LIBS) $(CRYPTO_FLAGS) $(MYSQL_FLAGS) -o LoginServer.o

ChatServer.o : ChatServer.cxx ChatServer.h Client.h Session.h server_utils.h
	$(CC) $(EXTRA_FLAGS) -c ChatServer.cxx $(LIBS) -o ChatServer.o

MatchMaker.o : MatchMaker.cxx MatchMaker.h server_utils.h
	$(CC) $(EXTRA_FLAGS) -c MatchMaker.cxx $(LIBS) -o MatchMaker.o

Session.o : Session.cxx Session.h Client.h server_utils.h
	$(CC) $(EXTRA_FLAGS) -c Session.cxx $(LIBS) -o Session.o

Client.o : Client.cxx Client.h
	$(CC) $(EXTRA_FLAGS) -c Client.cxx $(LIBS) -o Client.o


######## FUNCTION LIBRARY BUILD RULES


server_utils.o : server_utils.cxx server_utils.h
	$(CC) $(EXTRA_FLAGS) -c server_utils.cxx $(LIBS) $(CRYPTO_FLAGS) -o server_utils.o


######## ADDITIONAL RULES ########


clean:
	rm *.o launcher
