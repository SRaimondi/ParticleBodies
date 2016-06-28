#pragma once

// Includes
#include "matrix_include.h"

namespace pb {

	template <typename ELEMENT>
	class VoxelGrid {
	public:
		// Constructor
		VoxelGrid(size_t const x_dim, size_t const y_dim, size_t const z_dim,
				  math::vec3f const & grid_min, math::vec3f const & grid_max);

		// Destructor
		~VoxelGrid();

		// Set element value at a certain voxel
		void setElement(size_t const x, size_t const y, size_t const z, ELEMENT const & elem);

		// Get value in voxel
		ELEMENT const & getElement(size_t const x, size_t const y, size_t const z) const;

		// Get voxel center
		math::vec3f getVoxelCenter(size_t const x, size_t const y, size_t const z) const;

	protected:
		// Voxel grid dimensions
		size_t const x_dim;
		size_t const y_dim;
		size_t const z_dim;
		// Grid minimumm and maximum
		math::vec3f const grid_min;
		math::vec3f const grid_max;
		// Size of each voxel in the grid
		float const voxel_x;
		float const voxel_y;
		float const voxel_z;
		// Value in each voxel
		ELEMENT * voxels_content;
	};

	// VoxelGrid class methods implementation
	template <typename ELEMENT>
	VoxelGrid<ELEMENT>::VoxelGrid(size_t const x_dim, size_t const y_dim, size_t const z_dim,
								  math::vec3f const & grid_min, math::vec3f const & grid_max)
		: x_dim(x_dim), y_dim(y_dim), z_dim(z_dim),
		grid_min(grid_min), grid_max(grid_max) {
		// Allocate memory for content
		voxels_content = new ELEMENT[x_dim * y_dim * z_dim];
		// Compute voxels dimensions
		voxel_x = (grid_max(0) - grid_min(0)) / static_cast<float>(x_dim);
		voxel_y = (grid_max(1) - grid_min(1)) / static_cast<float>(y_dim);
		voxel_z = (grid_max(2) - grid_min(2)) / static_cast<float>(z_dim);
	}

	template <typename ELEMENT>
	VoxelGrid<ELEMENT>::~VoxelGrid() {
		delete[] voxels_content;
	}

	template <typename ELEMENT>
	void VoxelGrid<ELEMENT>::setElement(size_t const x, size_t const y, size_t const z, ELEMENT const & elem) {
#ifdef  _DEBUG
		assert(x >= 0 && x < x_dim);
		assert(y >= 0 && y < y_dim);
		assert(z >= 0 && z < z_dim);
#endif //  _DEBUG

		// Set element value
		voxels_content[x + x_dim * z + x_dim * z_dim * y] = elem;
	}

	template <typename ELEMENT>
	ELEMENT const & VoxelGrid<ELEMENT>::getElement(size_t const x, size_t const y, size_t const z) const {
#ifdef  _DEBUG
		assert(x >= 0 && x < x_dim);
		assert(y >= 0 && y < y_dim);
		assert(z >= 0 && z < z_dim);
#endif //  _DEBUG

		// Return element 
		return voxels_content[x + x_dim * z + x_dim * z_dim * y];
	}

	template <typename ELEMENT>
	math::vec3f VoxelGrid<ELEMENT>::getVoxelCenter(size_t const x, size_t const y, size_t const z) const {
#ifdef  _DEBUG
		assert(x >= 0 && x < x_dim);
		assert(y >= 0 && y < y_dim);
		assert(z >= 0 && z < z_dim);
#endif //  _DEBUG
		// Compute voxel center
		return (grid_min + (x + 0.5f) * voxel_x * math::vec3f({ 1.f, 0.f, 0.f }) +
			(y + 0.5f) * voxel_y * math::vec3f({ 0.f, 1.f, 0.f })) +
			(z + 0.5f) * voxel_z * math::vec3f({ 0.f, 0.f, 1.f });
	}

} // pb namespace