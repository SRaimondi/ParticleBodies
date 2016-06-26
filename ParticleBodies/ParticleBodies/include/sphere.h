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
		void generateBBOX(math::vec3f * min, math::vec3f * max) const;

		// Intersect a ray with the object, tells if the intersection is an entering or exiting from the object
		bool intersect(math::vec3f const & start, math::vec3f const & direction,
					   float const tmin, float const tmax,
					   bool * entering) const;

	private:
		// Sphere radius
		float const radius;
	};

} // pb namespace