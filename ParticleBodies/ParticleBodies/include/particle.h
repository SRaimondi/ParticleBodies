#pragma once

#include "matrix_include.h"

namespace pb {

	// Define particle class
	class Particle {
	public:
		// Constructor
		Particle(pb::math::vec3f const & p, float const radius);

		// Add force to particle
		void addForce(pb::math::vec3f const & f);

		// Reset particle forces
		void resetForce();

		// Friend declaration
		friend class BodyParticlesDiscretisation;

	protected:
		// Particle position
		pb::math::vec3f position;
		// Particle radius
		float const radius;
		// Particle sum of forces
		pb::math::vec3f force;
	};

} // pb namespace