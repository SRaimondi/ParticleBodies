#include "body.h"
#include "sphere_graphics.h"

namespace pb {

	Body::Body() {}

	Body::Body(math::vec3f const & cm) {
		// Set body center of mass
		physical_properties.x = cm;
	}

	Body::~Body() {}

	math::vec3f const & Body::getCenterOfMass() const {
		return physical_properties.x;
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

		// glPolygonMode(GL_FRONT, GL_LINE);
		glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_SHORT, (GLvoid*)0);

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