# GNU Makefile for Linux Server
# Peter Forbes 1/30/2016

######## VARS ########
CC = g++
LAUNCHER_DEPS = LoginServer.h
LAUNCHER_OBJS = LoginServer.o
EXTRA_FLAGS = -Wall -g -std=c++11
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

server_utils.o : server_utils.cxx server_utils.h
	$(CC) $(EXTRA_FLAGS) -c server_utils.cxx -o server_utils.o

######## ADDITIONAL RULES ########

clean:
	rm *.o launcher
