#include "body.h"
#include "sphere_graphics.h"

namespace pb {

	Body::Body() {}

	Body::Body(math::vec3f const & cm, float const mass,
			   math::quaternionf const & orientation) {
		// Set mass
		physical_properties.mass = mass;
		// Set body center of mass
		physical_properties.x = cm;
		// Set body orientation
		physical_properties.q = orientation;
	}

	Body::~Body() {}

	void Body::addForce(math::vec3f const & f) {
		physical_properties.force = physical_properties.force + f;
	}

	void Body::resetForce() {
		physical_properties.force(0) = 0;
		physical_properties.force(1) = 0;
		physical_properties.force(2) = 0;
	}

	void Body::addForceAsTorque(math::vec3f const & f, math::vec3f const & p) {
		physical_properties.torque = physical_properties.torque + math::crossProduct(f, p);
	}

	void Body::resetTorque() {
		physical_properties.torque(0) = 0;
		physical_properties.torque(1) = 0;
		physical_properties.torque(2) = 0;
	}

	math::vec3f const & Body::getCenterOfMass() const {
		return physical_properties.x;
	}

	math::mat4x4f Body::getOrientationMatrix() const {
		return math::createRotationMatrix(physical_properties.q);
	}

	math::vec3f Body::pointVelocityWorld(math::vec3f const & p) const {
		return (physical_properties.v + math::crossProduct(physical_properties.omega, p - physical_properties.x));
	}

	math::vec3f Body::pointVelocityLocal(math::vec3f const & p) const {
		return (math::crossProduct(physical_properties.omega, p));
	}

	void Body::drawBody(GLuint const v_buff,
						GLuint const i_buff,
						GLuint const num_elements) const {
		// Enable vertex and normal client state
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		// Bind buffers
		glBindBuffer(GL_ARRAY_BUFFER, v_buff);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buff);
		// Set data format
		glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)0);
		glNormalPointer(GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		// Preprocess
		drawPreprocess();

		//glPolygonMode(GL_FRONT, GL_LINE);
		glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_SHORT, (GLvoid*)0);
		glPolygonMode(GL_FRONT, GL_FILL);

		// Postprocess
		drawPostprocess();

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Disable client state
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

} // pb namespace