#include "Vertex.h"

uint16_t Vertex::count = 0;

Vertex::Vertex(int x0, int y0, int z0) : x(x0), y(y0), z(z0), id(count) {
	count++;
}

void Vertex::get_coords(int *x_coord, int* y_coord, int* z_coord) const {
	*x_coord = x;
	*y_coord = y;
	*z_coord = z;
}

uint16_t Vertex::get_id() const {
	return id;
}
