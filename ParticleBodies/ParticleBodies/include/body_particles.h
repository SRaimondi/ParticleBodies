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

		// Check if two object are colliding
		void colliding(BodyParticlesDiscretisation const & other) const;

		// Draw all particles
		void drawParticles(GLuint const sphere_v_buff,
						   GLuint const sphere_i_buff,
						   GLuint const num_elements) const;

		// Apply forces from particles to object
		void transferForcesParticlesBody();

		// Get pointer to the body
		Body * const getBody() const;

	private:
		// Generate body particles discretisation
		void generateParticles(float const particle_diameter,
							   bool const process_interior = true);

		// Resolve force between two particle of two different bodies that are colliding
		void solveContact(Particle & p1, Particle & p2) const;

		// Transform particle position to world space
		math::vec3f particlePositionWorld(Particle const & particle) const;

		// Physical body
		Body * const body;
		// List of particles
		std::vector<Particle> particles;
	};

} // pb namespace