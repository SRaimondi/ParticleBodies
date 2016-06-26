#include "body.h"

namespace pb {

	Body::Body()
		: center_of_mass(), body_particles() {}

	Body::Body(math::vec3f const & cm)
		: center_of_mass(cm), body_particles() {}

	Body::~Body() {}

} // pb namespace