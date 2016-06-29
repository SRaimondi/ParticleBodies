#pragma once

#include <GL/glew.h>
#include "particle.h"
#include <vector>

namespace pb {

	// Classes forward declaration
	class Body;
	class Particle;

	// This class defines the connection between a Body and his physical approximation
	class BodyParticlesDiscretisation {
	public:
		// Constructor
		BodyParticlesDiscretisation(Body * const body, float const particle_diameter);

		// Draw all particles
		void drawParticles(GLuint const sphere_v_buff,
						   GLuint const sphere_i_buff,
						   GLuint const num_elements) const;

	private:
		// Generate body particles discretisation
		void generateParticles(float const particle_diameter,
							   bool const process_interior = true);

		// Physical body
		Body * const body;
		// List of particles
		std::vector<Particle> particles;
	};

} // pb namespace