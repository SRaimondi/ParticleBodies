#pragma once

// Includes
#include "particle.h"
#include <vector>

namespace pb {

	// This class defines the basic body interface, all objects that run in the simulation
	// must extend this class and the virtual methods
	class Body {
	public:
		// Constructor
		Body();

		Body(math::vec3f const & cm);

		// Virtual destructor
		virtual ~Body();

		// This method is called before the simulation to generate the particle approximation for each object
		virtual void generateBodyParticles(float const particle_size) = 0;

	protected:
		// Generate BBOX of the object
		virtual void generateBBOX(math::vec3f * min, math::vec3f * max) const = 0;

		// Center of mass of the body
		math::vec3f center_of_mass;

		// List of particles belonging to the body, position of the particles is with respect
		// to the body center of mass
		std::vector<Particle> body_particles;
	};

} // pb namespace