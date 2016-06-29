#include "sphere.h"

namespace pb {

	Sphere::Sphere()
		: Body(), radius(1.f) {}

	Sphere::Sphere(math::vec3f const & cm, float const mass,
				   math::quaternionf const & orientation, float const radius)
		: Body(cm, mass, orientation), radius(radius) {
		// Compute inertia tensor of the body
		computeInertiaTensor();
	}

	void Sphere::generateBBOX(math::vec3f * min, math::vec3f * max) const {
		// Set BBOX minimum
		*min = math::vec3f({ physical_properties.x(0) - radius,
						   physical_properties.x(1) - radius,
						   physical_properties.x(2) - radius});

		// Set BBOX maximum
		*max = math::vec3f({ physical_properties.x(0) + radius,
						   physical_properties.x(1) + radius,
						   physical_properties.x(2) + radius});
	}

	bool Sphere::pointInside(math::vec3f const & p) const {
		// Compute distance
		float distance = math::magnitude(p - physical_properties.x);
		// Check if distance between center and voxel center is less than radius
		return (distance <= radius);
	}

	void Sphere::drawPreprocess() const {
		// Set matrix mode to model view
		glMatrixMode(GL_MODELVIEW);

		// Translate to center of mass
		glPushMatrix();
		// Translate to center of mass
		glTranslatef(physical_properties.x(0), physical_properties.x(1), physical_properties.x(2));
		// Scale using radius
		glScalef(radius, radius, radius);
		// Rotate using quaternion
		math::mat4x4f R = getOrientationMatrix();
		R = math::transpose(R);
		glMultMatrixf(&R(0, 0));

		// Set draw color
		glColor4f(1.f, 0.f, 0.f, 1.f);
	}

	void Sphere::drawPostprocess() const {
		// Set matrix mode to model view
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	void Sphere::computeInertiaTensor() {
		float inertia_tensor_entry = 2.f / 5.f * physical_properties.mass * radius * radius;
		for (size_t i = 0; i < 3; i++) {
			physical_properties.inertia_tensor_body(i, i) = inertia_tensor_entry;
			physical_properties.inv_inertia_tensor_body(i, i) = 1.f / inertia_tensor_entry;
		}
	}

} // pb namespace