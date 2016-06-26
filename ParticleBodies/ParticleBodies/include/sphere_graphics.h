#pragma once

// Includes
#include <GL\glew.h>

namespace pb {

	// Define class that creates the graphic for a sphere
	class SphereGraphic {
	public:
		static void createSphereGraphic(size_t const rings, size_t const sectors,
										GLuint * vertex_buff, GLuint * index_buff);
	};

} // pb namespace