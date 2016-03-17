# Test Makefile #

CC=g++
MAIN_DEPS = Unreliable_UDP_Socket.h
MAIN_OBJS = Socket.o UDP_Socket.o Secure_Socket.o Unreliable_UDP_Socket.o
CRYPTO_FLAGS = -lssl -lcrypto
EXTRA_FLAGS = -Wall -std=c++0x -g
LIBS = -L/usr/lib

main : main.o $(MAIN_OBJS)
	$(CC) main.o $(MAIN_OBJS) -o main $(CRYPTO_FLAGS)

main.o : main.cxx Unreliable_UDP_Socket.h
	$(CC) $(EXTRA_FLAGS) -c main.cxx -o main.o

Secure_Socket.o : Secure_Socket.cxx Secure_Socket.h Socket.h
	$(CC) $(EXTRA_FLAGS) -c Secure_Socket.cxx $(LIBS) $(CRYPTO_FLAGS) -o Secure_Socket.o

Unreliable_UDP_Socket.o : Unreliable_UDP_Socket.cxx Unreliable_UDP_Socket.h UDP_Socket.h
	$(CC) $(EXTRA_FLAGS) -c Unreliable_UDP_Socket.cxx -o Unreliable_UDP_Socket.o

UDP_Socket.o : UDP_Socket.cxx UDP_Socket.h Socket.h
	$(CC) $(EXTRA_FLAGS) -c UDP_Socket.cxx -o UDP_Socket.o

Socket.o : Socket.cxx Socket.h
	$(CC) $(EXTRA_FLAGS) -c Socket.cxx -o Socket.o

clean:
	rm *.o main

