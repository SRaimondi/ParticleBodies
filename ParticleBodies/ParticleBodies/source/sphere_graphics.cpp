#include "sphere_graphics.h"
#include <vector>
#include <cmath>

#define M_PI 3.1415926f

namespace pb {

	void SphereGraphic::createSphereGraphic(size_t const rings, size_t const sectors,
											GLuint * vertex_buff, GLuint * index_buff,
											GLuint * num_elements) {
		// Vertices and normals
		std::vector<GLfloat> vertices_normals;
		// Indices
		std::vector<GLushort> indices;

		// Create data
		float const R = 1.f / static_cast<float>(rings - 1);
		float const S = 1.f / static_cast<float>(sectors - 1);
		int r, s;

		// Resize buffers
		vertices_normals.resize(2 * rings * sectors * 3);

		// Get iterators to vertices and normals
		auto v = vertices_normals.begin();

		// Insert vertices and normal data into array
		for (r = 0; r < rings; r++) {
			for (s = 0; s < sectors; s++) {
				float const x = cosf(2.f * M_PI * s * S) * sinf(M_PI * r * R);
				float const y = cosf(M_PI * r * R);
				float const z = sinf(2.f * M_PI * s * S) * sinf(M_PI * r * R);

				// Insert vertex
				*v++ = x;
				*v++ = y;
				*v++ = z;

				// Normal
				*v++ = x;
				*v++ = y;
				*v++ = z;
			}
		}

		// Set number of element to draw
		*num_elements = rings * sectors * 6;
		// Create indices
		indices.resize(*num_elements);
		auto i = indices.begin();
		for (r = 0; r < rings - 1; r++) {
			for (s = 0; s < sectors - 1; s++) {
				// Insert indices
				//*i++ = r * sectors + s;
				//*i++ = r * sectors + (s + 1);
				//*i++ = (r + 1) * sectors + (s + 1);
				//*i++ = (r + 1) * sectors + s;
				// Insert first triangle
				*i++ = r * sectors + s;
				*i++ = r * sectors + (s + 1);
				*i++ = (r + 1) * sectors + (s + 1);
				// Insert second triangle
				*i++ = r * sectors + s;
				*i++ = (r + 1) * sectors + (s + 1);
				*i++ = (r + 1) * sectors + s;
			}
		}

		// Insert data into buffers
		glGenBuffers(1, vertex_buff);
		glGenBuffers(1, index_buff);

		// Copy vertex and normals into buffer
		glBindBuffer(GL_ARRAY_BUFFER, *vertex_buff);
		// Copy data to the GPU
		glBufferData(GL_ARRAY_BUFFER,
					 sizeof(GLfloat) * vertices_normals.size(),
					 &vertices_normals[0],
					 GL_STATIC_DRAW);
		// Unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Copy indices into buffer
		glBindBuffer(GL_ARRAY_BUFFER, *index_buff);
		// Copy data to the GPU
		glBufferData(GL_ARRAY_BUFFER,
					 sizeof(GLushort) * indices.size(),
					 &indices[0],
					 GL_STATIC_DRAW);
		// Unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Clear host buffer
		vertices_normals.clear();
		indices.clear();
	}

} // pb namespace