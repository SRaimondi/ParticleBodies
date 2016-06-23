/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   particle.hpp
 * Author: Simon
 *
 * Created on June 23, 2016, 6:06 PM
 */

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

// Include math header
#include "vmath.hpp"

namespace pb {
    // Define particle class
    class Particle {
    public:
        // Constructor
        Particle(vmath::vec3 const & p, float const r);
        
        // Add force to particle
        void applyForce(vmath::vec3 const & f);
        
        // Reset forces on particle
        void resetForce();
        
        // Check if two particles are colliding
        bool colliding(Particle const & other) const;
        
        // Get particle position
        vmath::vec3 const & getPosition() const;
        
        // Get particle force
        vmath::vec3 const & getForce() const;
        
    protected:
        // Particle position
        vmath::vec3 position;
        // Particle radius
        float const radius;
        // Sum of forces on particle
        vmath::vec3 forces_sum;
    };
}

#endif /* PARTICLE_HPP */

