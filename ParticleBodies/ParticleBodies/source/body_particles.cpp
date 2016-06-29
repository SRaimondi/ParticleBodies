#include "body_particles.h"
#include "body.h"
#include "voxel_grid.h"

namespace pb {

	BodyParticlesDiscretisation::BodyParticlesDiscretisation(Body * const body, float const particle_diameter)
		: body(body) {
		// Generate particles
		generateParticles(particle_diameter);
	}

	void BodyParticlesDiscretisation::generateParticles(float const particle_diameter,
														bool const process_interior) {
		// Request to body BBOX 
		math::vec3f min, max;
		body->generateBBOX(&min, &max);
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
		VoxelGrid<bool> voxel_grid = VoxelGrid<bool>(part_x, part_y, part_z, box_min, box_max);

		// Loop over all voxels and set voxel to true if inside the object
		for (size_t voxel_y = 0; voxel_y < part_y; voxel_y++) {
			for (size_t voxel_z = 0; voxel_z < part_z; voxel_z++) {
				for (size_t voxel_x = 0; voxel_x < part_x; voxel_x++) {
					// Get voxel center
					math::vec3f voxel_center = voxel_grid.getVoxelCenter(voxel_x, voxel_y, voxel_z);
					// Check if voxel is inside
					if (body->pointInside(voxel_center)) {
						voxel_grid.setElement(voxel_x, voxel_y, voxel_z, true);
					}
				}
			}
		}

		// Voxel grid processing
		if (process_interior) {
			voxel_grid.removeInteriorVoxels();
		}

		// Loop over all voxels set to true and add particle at that point
		for (size_t voxel_y = 0; voxel_y < part_y; voxel_y++) {
			for (size_t voxel_z = 0; voxel_z < part_z; voxel_z++) {
				for (size_t voxel_x = 0; voxel_x < part_x; voxel_x++) {
					// Check if voxel is inside object
					if (voxel_grid.getElement(voxel_x, voxel_y, voxel_z)) {
						// Compute voxel center
						math::vec3f voxel_center = voxel_grid.getVoxelCenter(voxel_x, voxel_y, voxel_z);
						// Add particle
						particles.push_back(Particle(voxel_center - body->getCenterOfMass(), particle_diameter / 2.f));
					}
				}
			}
		}
	}

	math::vec3f BodyParticlesDiscretisation::particlePositionWorld(Particle const & particle) const {
		return (body->getCenterOfMass() + math::transformLocation(body->getOrientationMatrix(), particle.position));
	}

	void BodyParticlesDiscretisation::drawParticles(GLuint const sphere_v_buff,
													GLuint const sphere_i_buff,
													GLuint const num_elements) const {
		// Set matrix mode to model view
		glMatrixMode(GL_MODELVIEW);

		// Translate to center of mass
		glPushMatrix();
		//glTranslatef(body->getCenterOfMass()(0), body->getCenterOfMass()(1), body->getCenterOfMass()(2));

		// Set draw color
		glColor3f(0.f, 0.f, 1.f);
		// Enable vertex and normal client state
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		// Bind buffers
		glBindBuffer(GL_ARRAY_BUFFER, sphere_v_buff);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_i_buff);
		// Set data format
		glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)0);
		glNormalPointer(GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		// Use list of particles to draw them
		for (auto p = particles.begin(); p != particles.end(); p++) {
			// Translate to particle position, wrt. center of mass
			glPushMatrix();
			math::vec3f particle_position = particlePositionWorld(*p);
			glTranslatef(particle_position(0), particle_position(1), particle_position(2));
			glScalef(p->radius, p->radius, p->radius);

			// glPolygonMode(GL_FRONT, GL_LINE);
			glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_SHORT, (GLvoid*)0);

			// Pop matrix from stack
			glPopMatrix();
		}

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Disable client state
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		// Pop matrix
		glPopMatrix();
	}

	void BodyParticlesDiscretisation::transferForcesParticlesBody() {
		// Loop over all particles
		for (auto particle = particles.begin(); particle != particles.end(); particle++) {
			// Apply force
			body->addForce(particle->force);
			// Apply torque
			body->addForceAsTorque(particle->force, particle->position);
		}
	}

} // pb namespace