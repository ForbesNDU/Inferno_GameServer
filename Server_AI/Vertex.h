#ifndef VERTEX_H
#define VERTEX_H

#include <stdint.h>

class Vertex {
	public:
		Vertex(int, int, int);	
		
		void get_coords(int*, int*, int*) const;
		uint16_t get_id() const;

	private:
		static uint16_t count;

		const int x;
		const int y;
		const int z;
                const uint16_t id;

};

#endif
