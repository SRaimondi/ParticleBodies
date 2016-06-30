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

		Body(math::vec3f const & cm, float const mass,
			 math::quaternionf const & orientation);

		// Virtual destructor
		virtual ~Body();

		// Add force to body
		void addForce(math::vec3f const & f);

		// Reset forces on body
		void resetForce();

		// Add force as torque
		void addForceAsTorque(math::vec3f const & f, math::vec3f const & p);

		// Reset torque
		void resetTorque();

		// Generate BBOX of the body
		virtual void generateBBOX(math::vec3f * min, math::vec3f * max) const = 0;

		// Check if a point is inside the object
		virtual bool pointInside(math::vec3f const & p) const = 0;

		// Get center of mass of the body
		math::vec3f const & getCenterOfMass() const;

		// Get rotation quaternion
		math::mat4x4f getOrientationMatrix() const;

		// Compute velocity of a point, in world space, on the body 
		math::vec3f pointVelocityWorld(math::vec3f const & p) const;

		// Compute velocity of a point in local space on the body
		math::vec3f pointVelocityLocal(math::vec3f const & p) const;

		// Copy the body state information to an array
		void bodyStateToArray(float y[]) const;

		// Copy the array into the object state
		void arrayToBodyState(float const y[]);

		// Copy state derivative to array
		void ddtBodyStateToArray(float y_dot[]) const;

		// Draw body
		void drawBody(GLuint const v_buff,
					  GLuint const i_buff,
					  GLuint const num_elements) const;

	protected:
		// Preprocessing needed for drawing
		virtual void drawPreprocess() const = 0;

		// Postprocessing after drawing
		virtual void drawPostprocess() const = 0;

		// Compute inertia tensor of the body
		virtual void computeInertiaTensor() = 0;

		// Body physical description
		PhysicalProperties physical_properties;
	};

} // pb namespace