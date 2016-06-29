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

} // pb namespace