#pragma once

// Includes
#include "particle.h"
#include "voxel_grid.h"
#include <vector>

// Define small BBOX tollerance
#define EPS 10e-4f

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

		// This method is called before the simulation to generate the particle approximation for each object
		//void generateBodyParticles(float const particle_diameter);

		// Generate BBOX of the body
		virtual void generateBBOX(math::vec3f * min, math::vec3f * max) const = 0;

		// Check if a point is inside the object
		virtual bool pointInside(math::vec3f const & p) const = 0;

		// DEBUG METHOD
		void drawAllParticles() const;

	protected:
		// Generate grid minimum and maximum for the grid
		//void generateGridMinMax(math::vec3f * grid_min, math::vec3f * grid_max, float const particle_diameter) const;

		// Generate VoxelGrid from object
		//virtual VoxelGrid<bool> generateVoxelGrid() const = 0;

		// List of particles belonging to the body, position of the particles is with respect
		// to the body center of mass
		//std::vector<Particle> body_particles;

		// DEBUG particle draw buffer
		unsigned int v_buff, i_buff;
	};

} // pb namespace