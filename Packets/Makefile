# Test Makefile #

######## DEFINITIONS ########

CC=g++
MAIN_DEPS = G_Ping_Packet.h
MAIN_OBJS = G_Ping_Packet.o Packet.o
EXTRA_FLAGS = -Wall -std=c++0x -g

######## MAIN RULES ########

main : main.o $(MAIN_OBJS)
	$(CC) main.o $(MAIN_OBJS) -o main

main.o : main.cxx $(MAIN_DEPS)
	$(CC) $(EXTRA_FLAGS) -c main.cxx -o main.o

######## PACKET RULES ########

Packet.o : Packet.cxx Packet.h
	$(CC) $(EXTRA_FLAGS) -c Packet.cxx -o Packet.o

######## STATE CHANGE PACKET RULES ########

G_Ping_Packet.o : G_Ping_Packet.cxx G_Ping_Packet.h Packet.h
	$(CC) $(EXTRA_FLAGS) -c G_Ping_Packet.cxx -o G_Ping_Packet.o

######## ADD'L RULES ########

clean:
	rm *.o main

