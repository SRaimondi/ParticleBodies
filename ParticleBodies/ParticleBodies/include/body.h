#pragma once

// Includes
#include "physical_properties.h"
#include <GL/glew.h>
#include <vector>

namespace pb {

	// This class defines the basic body interface, all objects that run in the simulation
	// must extend this class and the virtual methods
	class Body {
	public:
		// Constructor
		Body();

		Body(math::vec3f const & cm);

		// Virtual destructor
		virtual ~Body();

		// Generate BBOX of the body
		virtual void generateBBOX(math::vec3f * min, math::vec3f * max) const = 0;

		// Check if a point is inside the object
		virtual bool pointInside(math::vec3f const & p) const = 0;

		// Get center of mass of thebody
		math::vec3f const & getCenterOfMass() const;

		// Draw body
		void drawBody(GLuint const v_buff,
					  GLuint const i_buff,
					  GLuint const num_elements) const;

	protected:
		// Preprocessing needed for drawing
		virtual void drawPreprocess() const = 0;

		// Postprocessing after drawing
		virtual void drawPostprocess() const = 0;

		// Body physical description
		PhysicalProperties physical_properties;
	};

} // pb namespace