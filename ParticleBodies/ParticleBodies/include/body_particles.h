#pragma once

#include "body.h"
#include "particle.h"
#include <vector>
#include <GL\glew.h>

namespace pb {

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
		void generateParticles(float const particle_diameter);

		// Physical body
		Body * const body;
		// List of particles
		std::vector<Particle> particles;
	};

} // pb namespace