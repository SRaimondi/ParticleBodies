#include "body_particles.h"
#include "voxel_grid.h"

namespace pb {

	BodyParticlesDiscretisation::BodyParticlesDiscretisation(Body * const body, float const particle_diameter)
		: body(body) {
		// Generate particles
		generateParticles(particle_diameter);
	}

	void BodyParticlesDiscretisation::generateParticles(float const particle_diameter) {
		// Request to body BBOX 
		math::vec3f min, max;
		// generateBBOX(&min, &max);
		// Compute BBOX dims
		math::vec3f dim = max - min;
		// Find how many particle's boxes we have in each direction
		size_t const part_x = static_cast<size_t>(ceil(dim(0) / particle_diameter));
		size_t const part_y = static_cast<size_t>(ceil(dim(1) / particle_diameter));
		size_t const part_z = static_cast<size_t>(ceil(dim(2) / particle_diameter));
		// Check how much we are outside of the BBOX limits
		math::vec3f offset = math::vec3f({ part_x * particle_diameter,
										 part_y  * particle_diameter,
										 part_z * particle_diameter });
		// Subtract BBOX dimensions
		offset = offset - dim;
		// Compute boxes minimum and maximum
		math::vec3f const box_min = min - (0.5f * offset);
		math::vec3f const box_max = max + (0.5f * offset);

		// Create voxel grid
		VoxelGrid<bool> grid = VoxelGrid<bool>(part_x, part_y, part_z, box_min, box_max);

		// Loop over all voxels and set voxel to true if inside the object
		for (size_t voxel_y = 0; voxel_y < part_y; voxel_y++) {
			for (size_t voxel_z = 0; voxel_z < part_z; voxel_z++) {
				for (size_t voxel_x = 0; voxel_x < part_x; voxel_x++) {
					// Get voxel center
					math::vec3f voxel_center = grid.getVoxelCenter(voxel_x, voxel_y, voxel_z);
					// Check if voxel is inside
					if (body->pointInside(voxel_center)) {
						grid.setElement(voxel_x, voxel_y, voxel_z, true);
					} else {
						grid.setElement(voxel_x, voxel_y, voxel_z, false);
					}
				}
			}
		}
	}

} // pb namespace