#include "body.h"
#include "sphere_graphics.h"

namespace pb {

	Body::Body()
		: center_of_mass(), body_particles() {}

	Body::Body(math::vec3f const & cm)
		: center_of_mass(cm), body_particles() {
		// Generate graphics
		SphereGraphic::createSphereGraphic(20, 20, &v_buff, &i_buff);
	}

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
										 part_y  * particle_diameter,
										 part_z * particle_diameter });
		// Subtract BBOX dimensions
		offset = offset - dim;
		// Compute boxes minimum and maximum
		math::vec3f const box_min = min - (0.5f * offset);
		math::vec3f const box_max = max + (0.5f * offset);

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
					//math::vec3f const ray_start = box_min + 
					//	(static_cast<float>(voxel_x) - EPS) * voxel_x_dir +
					//	(static_cast<float>(voxel_y) + 0.5f) * voxel_y_dir +
					//	(static_cast<float>(voxel_z) + 0.5f) * voxel_z_dir;

					//// Check intersection with object
					//bool entering = false;
					//// Check if we intersect the object
					//if (intersect(ray_start, math::normalize(voxel_x_dir), 0.f, particle_diameter, &entering)) {
					//	// Check if we are entering
					//	if (entering) {
					//		// Add particle
					//		body_particles.push_back(Particle((ray_start + 0.5f * voxel_x_dir) - center_of_mass, particle_diameter / 2.f));
					//		// Set we are now inside the object
					//		inside_object = true;
					//	} else {
					//		// Add particle
					//		body_particles.push_back(Particle((ray_start + 0.5f * voxel_x_dir) - center_of_mass, particle_diameter / 2.f));
					//		// Set we are going out of the object
					//		inside_object = false;
					//	}

					//// Check if we are already inside the object so we need to add a particle
					//} else if (inside_object) {
					//	// Add particle
					//	body_particles.push_back(Particle((ray_start + 0.5f * voxel_x_dir) - center_of_mass, particle_diameter / 2.f));
					//}
					// Go to next voxel

					// Compute voxel center
					math::vec3f const voxel_center = box_min + 
						(static_cast<float>(voxel_x) + 0.5f) * voxel_x_dir +
						(static_cast<float>(voxel_y) + 0.5f) * voxel_y_dir +
						(static_cast<float>(voxel_z) + 0.5f) * voxel_z_dir;

					// Check if voxel is inside
					if (voxelInside(voxel_center, particle_diameter)) {
						body_particles.push_back(Particle(voxel_center - center_of_mass, particle_diameter / 2.f));
					}
				}
			}
		}
	}

	void Body::drawAllParticles() const {
		// Set matrix mode to model view
		glMatrixMode(GL_MODELVIEW);

		// Translate to center of mass
		glPushMatrix();
		glTranslatef(center_of_mass(0), center_of_mass(1), center_of_mass(2));

		// Use list of particles to draw them
		for (auto p = body_particles.begin(); p != body_particles.end(); p++) {
			// Translate to particle position
			glPushMatrix();
			glTranslatef(p->position(0), p->position(1), p->position(2));
			glScalef(p->radius, p->radius, p->radius);

			// Draw particle
			glColor3f(1.f, 1.f, 1.f);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, v_buff);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buff);
			glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)0);
			glNormalPointer(GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			//glPolygonMode(GL_FRONT, GL_LINE);
			glDrawElements(GL_QUADS, 20 * 20 * 4, GL_UNSIGNED_SHORT, (GLvoid*)0);

			// Pop matrix form stack
			glPopMatrix();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
		}

		glPopMatrix();
	}

} // pb namespace