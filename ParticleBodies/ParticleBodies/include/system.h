#pragma once

// Includes
#include "body_particles.h"

namespace pb {

	// Define class that olds the object running in the simulation
	class System {
	public:
		// Constructor
		System(float const t0, float const dt);

		// Add body to the system
		void addBody(BodyParticlesDiscretisation * body);

		// Compute forces and torque of the system
		void computeForceAndTorque(float const time) const;

		// Compute one step of the system
		void computeStep();

		// Draw system
		void draw(GLuint const v_p, GLuint const i_p, GLuint const n_p,
				  GLuint const v_s, GLuint const i_s, GLuint const n_s) const;

	private:
		// List of bodies stored using their particle representation
		std::vector<BodyParticlesDiscretisation *> bodies;
		// Current time of the system
		float t;
		// Step for the simulation
		float delta_t;
	};
} // pb namespace