# GNU Makefile for client code
# Peter Forbes 3/6/2016


######## VARS ########


CC = g++
EXTRA_FLAGS = -Wall -g
CRYPTO_FLAGS = -lssl -lcrypto


######## LAUNCHER BUILD RULES ########


client : client.o
	$(CC) client.o -o client $(CRYPTO_FLAGS)

client.o : client.cxx
	$(CC) -c client.cxx -o client.o $(EXTRA_FLAGS)


######## ADDITIONAL RULES ########


clean:
	rm *.o client
