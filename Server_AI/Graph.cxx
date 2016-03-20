#include "Graph.h"

////////// TOP LEVEL //////////

Graph::Graph() {
	graph = new std::vector< std::pair< Vertex*, std::list<Vertex*>* > >();
}

Graph::Graph( std::vector< std::pair< Vertex*, std::list<Vertex*>* > >* initial_graph ) {
	graph = initial_graph;
}

Graph::Graph(std::string vl_path, std::string al_path) {

	// Init data structs
        graph = new std::vector< std::pair< Vertex*, std::list<Vertex*>* > >();

	// Load them with file data if formatted correctly
	if(!parse_vl_file(vl_path)) {
		throw VL_PARSE_EXCEPTION;
	}
	if(!parse_al_file(al_path)) {
		throw AL_PARSE_EXCEPTION;
	}
	
}

Graph::~Graph() {
	delete graph;
}

////////// PUBLIC METHODS //////////

uint16_t Graph::add_vertex(int x, int y, int z) {

	// Create new vertex and null adjacency list
	Vertex *v = new Vertex(x, y, z);
	std::list<Vertex*>* adj_list = new std::list<Vertex*>();

	// Add to graph and return id
	graph->push_back( std::make_pair(v, adj_list) );

	return v->get_id();

}

void Graph::add_edge(uint16_t id_0, uint16_t id_1) {

	// Fetch vertices
	Vertex *v0 = (graph->at(id_0)).first;
	Vertex *v1 = (graph->at(id_1)).first;

	// Fetch adjacency lists (left out edge existence check for speedup)
	std::list<Vertex*>* adj0 = (graph->at(id_0)).second;
	std::list<Vertex*>* adj1 = (graph->at(id_1)).second;

	adj0->push_back(v1);
	adj1->push_back(v0);

}

std::list<Vertex*>* Graph::A_star(uint16_t id_0, uint16_t id_1) {

	Vertex *v0 = (graph->at(id_0)).first;
        Vertex *v1 = (graph->at(id_1)).first;

}

////////// PRIVATE METHODS //////////

bool Graph::parse_vl_file( std::string vl_path ) {

	// Attempt to open file
	FILE* vl = fopen(vl_path.c_str(), "r");
	if(vl == NULL) {
		return false;
	}

	// Read in each line and add each vertex to the graph structure
	char* line;
	size_t len;
	while(getline(&line, &len, vl)) {

		int x, y, z;
		char* token;

		token = strtok(line, ",\n");
		x = atoi(token);
		token = strtok(NULL, ",\n");
		y = atoi(token);
		token = strtok(NULL, ",\n");
		z = atoi(token);

		add_vertex(x, y, z);

	}

	return true;
}

bool Graph::parse_al_file( std::string al_path ) {

	// Attempt to open file
	FILE* al = fopen(al_path.c_str(), "r");
	if(al == NULL) {
		return false;
	}

        // Read in each line and add each vertex to the graph structure
	char* line;
	size_t len;
        while(getline(&line, & len, al)) {

		int id_0, id_1;
		char* token;

		token = strtok(line, ",\n");
		id_0 = atoi(token);
		token = strtok(line, ",\n");
		id_1 = atoi(token);

		add_edge(id_0, id_1);		

	}

	return true;

}

int Graph::get_distance(uint16_t id_0, uint16_t id_1) {

	Vertex *v0 = (graph->at(id_0)).first;
	Vertex *v1 = (graph->at(id_1)).first;

	int x0, y0, z0;
	v0->get_coords(&x0, &y0, &z0);

	int x1, y1, z1;
	v1->get_coords(&x1, &y1, &z1);

	int summation = pow((x0 - x1), 2) + pow((y0 - y1), 2) + pow((z0 - z1), 2);
	return sqrt(summation); 

}

int Graph::get_distance(uint16_t id, int x1, int y1, int z1) {

	Vertex *v = (graph->at(id)).first;

	int x0, y0, z0;
	v->get_coords(&x0, &y0, &z0);

        int summation = pow((x0 - x1), 2) + pow((y0 - y1), 2) + pow((z0 - z1), 2);
        return sqrt(summation);

}

void Graph::get_closest_vertex(uint16_t *response_id, uint16_t vert_id) {

	// Set defaults
	int min_distance = INT_MAX;	
	uint16_t closest_id = vert_id;

	// Get vertex and adjacency list
	Vertex *vert = (graph->at(vert_id)).first;
	std::list<Vertex*>* adjacency_list = (graph->at(vert_id)).second;	

	// Search adjacency list
	std::list<Vertex*>::iterator it;
	for(it = adjacency_list->begin(); it != adjacency_list->end(); ++it) {

		Vertex *current = *it;
		int distance = get_distance(current->get_id(), vert->get_id());		

		if(distance < min_distance) {
			min_distance = distance;
			closest_id = current->get_id();
		}

	}	

	// Return results
	*response_id = closest_id;

}

void Graph::get_closest_vertex(uint16_t *response_id, int x, int y, int z) {

	// Set defaults
	int min_distance = INT_MAX;
	uint16_t closest_id;

	// Search all vertices
	std::vector< std::pair< Vertex*, std::list<Vertex*>* > >::iterator it;
	for(it = graph->begin(); it != graph->end(); ++it) {

		Vertex* current = (*it).first;
		int distance = get_distance( current->get_id(), x, y, z );

		if(distance < min_distance) {
			min_distance = distance;
			closest_id = current->get_id();
		}
	
	}	

	// Return results	
	*response_id = closest_id;

}


















