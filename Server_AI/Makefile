# Test Makefile #

######## DEFINITIONS ########

CC=g++
MAIN_DEPS = Graph.h
MAIN_OBJS = Graph.o Vertex.o
EXTRA_FLAGS = -Wall -std=c++0x -g

######## MAIN RULES ########

main : main.o $(MAIN_OBJS)
	$(CC) main.o $(MAIN_OBJS) -o main

main.o : main.cxx Vertex.h
	$(CC) $(EXTRA_FLAGS) -c main.cxx -o main.o

######## GRAPH RULES ########

Graph.o : Graph.cxx Graph.h Vertex.h
	$(CC) $(EXTRA_FLAGS) -c Graph.cxx -o Graph.o

Vertex.o : Vertex.cxx Vertex.h
	$(CC) $(EXTRA_FLAGS) -c Vertex.cxx -o Vertex.o

######## ADD'L RULES ########

clean:
	rm *.o main
