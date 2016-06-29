#pragma once

// Includes
#include "matrix_include.h"
#include "quaternion.h"

namespace pb {

	// Define struct that holds the physical properties of a rigid body
	struct PhysicalProperties {
		// Define size of state
		static size_t const STATE_SIZE = 13;

		//
		// Constant quantities
		//
		// Mass
		float mass;
		// Inertia tensor of the body
		math::mat3x3f inertia_tensor_body;
		// Inverse of inertia tensor
		math::mat3x3f inv_inertia_tensor_body;

		//
		// State variables
		//
		// Position
		math::vec3f x;
		// Orientation
		math::Quaternion<float> q;
		// Linear and angular momentum
		math::vec3f P;
		math::vec3f L;

		//
		// Derived quantities
		//
		// Inverse of inertia tensor
		math::mat3x3f inv_inertia_tensor;
		// Orientation matrix
		math::mat3x3f R;
		// Velocity
		math::vec3f v;
		// Rotation vector
		math::vec3f omega;

		//
		// Compute quantities on body
		//
		// Force on body
		math::vec3f force;
		// Torque on body
		math::vec3f torque;
	};

	// Copy the state information into an array
	void stateToArray(PhysicalProperties const * const rb, float * y);

	// Copy array into object state
	void arrayToState(PhysicalProperties * const rb, float const * y);

	// State derivative to array
	void ddtStateToArray(PhysicalProperties const * const rb, float * y_dot);

} // pb namespace