#include "particle.h"

namespace pb {

	Particle::Particle(pb::math::vec3f const & p, float const radius) 
		: position(p), radius(radius), force() {}

	void Particle::addForce(pb::math::vec3f const & f) {
		force = force + f;
	}

	void Particle::resetForce() {
		force(0) = 0.f;
		force(1) = 0.f;
		force(2) = 0.f;
	}

	bool Particle::colliding(Particle const & other) const {
		return (pb::math::magnitude(other.position - position) <= radius + other.radius);
	}

} // pb namespace