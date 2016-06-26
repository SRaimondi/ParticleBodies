#include "body.h"

namespace pb {

	Body::Body()
		: center_of_mass(), body_particles() {}

	Body::Body(math::vec3f const & cm)
		: center_of_mass(cm), body_particles() {}

	Body::~Body() {}

	void Body::generateBodyParticles(float const particle_diameter) {
		// Generate BBOX data of the object
		math::vec3f min, max;
		generateBBOX(&min, &max);
		// Compute BBOX dims
		math::vec3f dim = max - min;
		// Find how many particle's boxes we have in each direction
		size_t const part_x = static_cast<size_t>(ceil(dim(0) / particle_diameter));
		size_t const part_y = static_cast<size_t>(ceil(dim(1) / particle_diameter));
		size_t const part_z = static_cast<size_t>(ceil(dim(2) / particle_diameter));
		// Check how much we are outside of the BBOX limits
		math::vec3f offset = math::vec3f({ part_x * particle_diameter,
										 part_y * particle_diameter,
										 part_z * particle_diameter });
		// Subtract BBOX dimensions
		offset = offset - dim;
		// Compute boxes minimum and maximum
		math::vec3f const box_min = min - (offset * 0.5f);
		math::vec3f const box_max = max + (offset * 0.5f);

		// Voxel directional vector
		math::vec3f const voxel_x_dir = math::vec3f({ particle_diameter, 0.f, 0.f });
		math::vec3f const voxel_y_dir = math::vec3f({ 0.f, particle_diameter, 0.f });
		math::vec3f const voxel_z_dir = math::vec3f({ 0.f, 0.f, particle_diameter });

		// Keep track if we are inside the object
		bool inside_object = false;
		// Loop over all voxels and add particle if voxel center is inside the object
		for (size_t voxel_y = 0; voxel_y < part_y; voxel_y++) {
			for (size_t voxel_z = 0; voxel_z < part_z; voxel_z++) {
				for (size_t voxel_x = 0; voxel_x < part_x; voxel_x++) {
					// Compute ray start
					math::vec3f const ray_start = box_min + static_cast<float>(voxel_x) * voxel_x_dir
						+ static_cast<float>(voxel_y) * 0.5f * voxel_y_dir + static_cast<float>(voxel_z) * 0.5f * voxel_z_dir;

					// Check intersection with object
					bool entering = false;
					float t;
					// Check if we intersect the object
					if (intersect(ray_start, voxel_x_dir, 0.f, 1.f, &t, &entering)) {
						// Check if we are entering
						if (entering) {
							// Check if intersection is before half
							if (t >= 0.5f) {
								// Add particle
								body_particles.push_back(Particle((ray_start + 0.5f * voxel_x_dir) - center_of_mass, particle_diameter / 2.f));
							}
							// Set we are now inside the object
							inside_object = true;
						} else {
							// We are exiting the object, check if intersection is before half
							if (t <= 0.5f) {
								// Add particle
								body_particles.push_back(Particle((ray_start + 0.5f * voxel_x_dir) - center_of_mass, particle_diameter / 2.f));
							}
							// Set we are outside the object
							inside_object = false;
						}
					} else if (inside_object) {
						// Add particle
						body_particles.push_back(Particle((ray_start + 0.5f * voxel_x_dir) - center_of_mass, particle_diameter / 2.f));
					}
					// Go to next voxel
				}
			}
		}
	}

} // pb namespace