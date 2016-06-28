#pragma once

#include "body.h"
#include "particle.h"
#include <vector>

namespace pb {

	// This class defines the connection between a Body and his physical approximation
	class BodyParticlesDiscretisation {
	public:
		// Constructor
		BodyParticlesDiscretisation(Body * const body, float const particle_diameter);

	private:
		// Generate body particles discretisation
		void generateParticles(float const particle_diameter);

		// Physical body
		Body * const body;
		// List of particles
		std::vector<Particle> particles;
	};

} // pb namespace