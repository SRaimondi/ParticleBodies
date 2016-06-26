#include "body.h"

namespace pb {

	Body::Body()
		: center_of_mass(), body_particles() {}

	Body::Body(math::vec3f const & cm)
		: center_of_mass(cm), body_particles() {}

	Body::~Body() {}

	void Body::generateBodyParticles(float const particle_size) {
		// Generate BBOX data of the object
		math::vec3f min, max;
		generateBBOX(&min, &max);
		// Compute BBOX dims
		math::vec3f dim = max - min;
		// Find how many particle's boxes we have in each direction
		size_t const part_x = static_cast<size_t>(ceil(dim(0) / particle_size));
		size_t const part_y = static_cast<size_t>(ceil(dim(1) / particle_size));
		size_t const part_z = static_cast<size_t>(ceil(dim(2) / particle_size));
		// Check how much we are outside of the BBOX limits
		math::vec3f offset = math::vec3f({ part_x * particle_size,
										 part_y * particle_size,
										 part_z * particle_size });
		// Subtract BBOX dimensions
		offset = offset - dim;
		// Compute boxes minimum and maximum
		math::vec3f box_min = min;
		math::vec3f box_max = max;
		// Shift min and max
		min = min - (offset * 0.5f);
		max = max - (offset * 0.5f);
	}

} // pb namespace