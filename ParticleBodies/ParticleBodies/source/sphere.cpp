#include "sphere.h"

namespace pb {

	Sphere::Sphere()
		: Body(), radius(1.f) {}

	Sphere::Sphere(math::vec3f const & cm, float const radius)
		: Body(cm), radius(radius) {}

	void Sphere::generateBBOX(math::vec3f * min, math::vec3f * max) const {
		// Set BBOX minimum
		*min = math::vec3f({ center_of_mass(0) - radius,
						   center_of_mass(1) - radius,
						   center_of_mass(2) - radius});

		// Set BBOX maximum
		*max = math::vec3f({ center_of_mass(0) + radius,
						   center_of_mass(1) + radius,
						   center_of_mass(2) + radius});
	}

	bool Sphere::intersect(math::vec3f const & start, math::vec3f const & direction,
						   float const tmin, float const tmax,
						   bool * entering) const {
		// Algorithm from Real-Time Rendering
		float r_2 = radius * radius;
		math::vec3f l = center_of_mass - start;
		float s = math::dotProduct(l, direction);
		float l_2 = math::dotProduct(l, l);
		// Check if we are outside pointing away
		if (s < 0.f && l_2 > r_2) {
			return false;
		}
		float m_2 = l_2 - s * s;
		if (m_2 > r_2) {
			return false;
		}
		// Intersection parameter
		float t;
		float q = sqrt(r_2 - m_2);
		if (l_2 > r_2) {
			// We are outside
			t = s - q;
			*entering = true;
		} else {
			t = s + q;
			*entering = false;
		}

		// Check for ray boundaries
		if (t < tmin || t > tmax) {
			return false;
		}

		// Intersection found
		return true;
	}

	bool Sphere::voxelInside(math::vec3f const & voxel_center, float const ) const {
		// Check if distance between center and voxel center is less than radius
		return (math::magnitude(voxel_center - center_of_mass) < radius);
	}

} // pb namespace