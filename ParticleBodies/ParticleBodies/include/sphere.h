#pragma once

// Includes
#include "body.h"

namespace pb {

	class Sphere : public Body {
	public:
		// Constructor
		Sphere();

		Sphere(math::vec3f const & cm, float const mass,
			   math::quaternionf const & orientation, float const radius);

		// Generate BBOX of the object
		void generateBBOX(math::vec3f * min, math::vec3f * max) const override;

		// Check if voxel is inside the body
		bool pointInside(math::vec3f const & p) const override;

	private:
		// Preprocessing needed for drawing
		void drawPreprocess() const override;

		// Postprocessing after drawing
		void drawPostprocess() const override;

		// Compute inertia tensor of the body
		void computeInertiaTensor() override;

		// Sphere radius
		float const radius;
	};

} // pb namespace