#pragma once

// Includes
#include "body.h"

namespace pb {

	class Sphere : public Body {
	public:
		// Constructor
		Sphere();

		Sphere(math::vec3f const & cm, float const radius);

	protected:
		// Generate BBOX of the object
		void generateBBOX(math::vec3f * min, math::vec3f * max) const override;

		// Check if voxel is inside the body
		bool pointInside(math::vec3f const & p) const override;

	private:
		// Sphere radius
		float const radius;
	};

} // pb namespace