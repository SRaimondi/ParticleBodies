/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "particle.hpp"

namespace pb {
    
    Particle::Particle(vmath::vec3 const & p, const float r) 
    : position(p), radius(r), forces_sum() {}

    void Particle::applyForce(vmath::vec3 const & f) {
        forces_sum += f;
    }

    void Particle::resetForce() {
        forces_sum[0] = 0.f;
        forces_sum[1] = 0.f;
        forces_sum[2] = 0.f;
    }

    bool Particle::colliding(Particle const & other) const {
        // Check if distance is less then diameter
        return (vmath::length(other.position - position) < radius + other.radius);
    }
    
    const vmath::vec3& Particle::getPosition() const {
        return (position);
    }

    const vmath::vec3& Particle::getForce() const {
        return (forces_sum);
    }

} // pb namespace