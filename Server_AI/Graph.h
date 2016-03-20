#ifndef GRAPH_H
#define GRAPH_H

#include "Vertex.h"

#include <limits.h>
#include <list>
#include <math.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>


typedef enum graph_exception {
	VL_PARSE_EXCEPTION,
	AL_PARSE_EXCEPTION
} type_exception;

class Graph {
	public:
		Graph();
		Graph( std::vector< std::pair< Vertex*, std::list<Vertex*>* > >* );
		Graph( std::string, std::string );
		~Graph();

		// Building	
		uint16_t add_vertex(int, int, int);
		void add_edge(uint16_t, uint16_t);

		// Pathfinding
		std::list<Vertex*>* A_star(uint16_t, uint16_t);

	private:
		// Internal Representation
		std::vector< std::pair<Vertex*, std::list<Vertex*>* > >* graph;	

		// Load from file utils
                bool parse_vl_file(std::string);
		bool parse_al_file(std::string);

		// Distance utilities
                int get_distance(uint16_t, uint16_t);
                int get_distance(uint16_t, int, int, int);
		void get_closest_vertex(uint16_t*, uint16_t);
		void get_closest_vertex(uint16_t*, int, int, int);

};

#endif
