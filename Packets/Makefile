# Test Makefile #

######## DEFINITIONS ########

CC=g++
MAIN_DEPS = packet_defs.h
MAIN_OBJS = Packet.o \
	    Event_Packet.o Sync_Packet.o Protocol_Packet.o \
	    C_Ping_Packet.o C_SYN_Packet.o \
	    G_Auto_Notify_Packet.o G_Auto_Request_Packet.o G_Move_Notify_Packet.o G_Move_Request_Packet.o \
	    G_Primary_Notify_Packet.o G_Primary_Request_Packet.o G_Ping_Packet.o \
	    I_Auto_Packet.o I_LastHit_Packet.o I_Local_Player_Packet.o I_Minion_Packet.o I_Objective_Packet.o
EXTRA_FLAGS = -Wall -std=c++0x -g

######## MAIN RULES ########

main : main.o $(MAIN_OBJS)
	$(CC) main.o $(MAIN_OBJS) -o main

main.o : main.cxx $(MAIN_DEPS)
	$(CC) $(EXTRA_FLAGS) -c main.cxx -o main.o

######## PACKET LAYER 1 ########

Packet.o : Packet.cxx Packet.h
	$(CC) $(EXTRA_FLAGS) -c Packet.cxx -o Packet.o

######## PACKET LAYER 2 ########

Event_Packet.o : Event_Packet.cxx Event_Packet.h Packet.h
	$(CC) $(EXTRA_FLAGS) -c Event_Packet.cxx -o Event_Packet.o

Protocol_Packet.o : Protocol_Packet.cxx Protocol_Packet.h Packet.h
	$(CC) $(EXTRA_FLAGS) -c Protocol_Packet.cxx -o Protocol_Packet.o

Sync_Packet.o : Sync_Packet.cxx Sync_Packet.h Packet.h
	$(CC) $(EXTRA_FLAGS) -c Sync_Packet.cxx -o Sync_Packet.o

######## PROTOCOL LAYER ########

C_Ping_Packet.o : C_Ping_Packet.cxx C_Ping_Packet.h Protocol_Packet.h
	$(CC) $(EXTRA_FLAGS) -c C_Ping_Packet.cxx -o C_Ping_Packet.o

C_SYN_Packet.o : C_SYN_Packet.cxx C_SYN_Packet.h Protocol_Packet.h
	$(CC) $(EXTRA_FLAGS) -c C_SYN_Packet.cxx -o C_SYN_Packet.o

######## EVENT LAYER ########

G_Auto_Notify_Packet.o : G_Auto_Notify_Packet.cxx G_Auto_Notify_Packet.h Event_Packet.h
	$(CC) $(EXTRA_FLAGS) -c G_Auto_Notify_Packet.cxx -o G_Auto_Notify_Packet.o

G_Auto_Request_Packet.o : G_Auto_Request_Packet.cxx G_Auto_Request_Packet.h Event_Packet.h
	$(CC) $(EXTRA_FLAGS) -c G_Auto_Request_Packet.cxx -o G_Auto_Request_Packet.o

G_Move_Notify_Packet.o : G_Move_Notify_Packet.cxx G_Move_Notify_Packet.h Event_Packet.h
	$(CC) $(EXTRA_FLAGS) -c G_Move_Notify_Packet.cxx -o G_Move_Notify_Packet.o

G_Move_Request_Packet.o : G_Move_Request_Packet.cxx G_Move_Request_Packet.h Event_Packet.h
	$(CC) $(EXTRA_FLAGS) -c G_Move_Request_Packet.cxx -o G_Move_Request_Packet.o

G_Ping_Packet.o : G_Ping_Packet.cxx G_Ping_Packet.h Event_Packet.h
	$(CC) $(EXTRA_FLAGS) -c G_Ping_Packet.cxx -o G_Ping_Packet.o

G_Primary_Notify_Packet.o : G_Primary_Notify_Packet.cxx G_Primary_Notify_Packet.h Event_Packet.h
	$(CC) $(EXTRA_FLAGS) -c G_Primary_Notify_Packet.cxx -o G_Primary_Notify_Packet.o

G_Primary_Request_Packet.o : G_Primary_Request_Packet.cxx G_Primary_Request_Packet.h Event_Packet.h
	$(CC) $(EXTRA_FLAGS) -c G_Primary_Request_Packet.cxx -o G_Primary_Request_Packet.o

######## SYNC LAYER ########

I_Auto_Packet.o : I_Auto_Packet.cxx I_Auto_Packet.h Sync_Packet.h
	$(CC) $(EXTRA_FLAGS) -c I_Auto_Packet.cxx -o I_Auto_Packet.o

I_LastHit_Packet.o : I_LastHit_Packet.cxx I_LastHit_Packet.h Sync_Packet.h
	$(CC) $(EXTRA_FLAGS) -c I_LastHit_Packet.cxx -o I_LastHit_Packet.o

I_Local_Player_Packet.o : I_Local_Player_Packet.cxx I_Local_Player_Packet.h Sync_Packet.h
	$(CC) $(EXTRA_FLAGS) -c I_Local_Player_Packet.cxx -o I_Local_Player_Packet.o

I_Minion_Packet.o : I_Minion_Packet.cxx I_Minion_Packet.h Sync_Packet.h
	$(CC) $(EXTRA_FLAGS) -c I_Minion_Packet.cxx -o I_Minion_Packet.o

I_Objective_Packet.o : I_Objective_Packet.cxx I_Objective_Packet.h Sync_Packet.h
	$(CC) $(EXTRA_FLAGS) -c I_Objective_Packet.cxx -o I_Objective_Packet.o

I_Primary_Packet.o : I_Primary_Packet.cxx I_Primary_Packet.h Sync_Packet.h
	$(CC) $(EXTRA_FLAGS) -c I_Primary_Packet.cxx -o I_Primary_Packet.o

######## ADD'L RULES ########

clean:
	rm *.o main





















