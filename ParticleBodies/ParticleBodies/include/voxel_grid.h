#pragma once

// Includes
#include "matrix_include.h"
#include <vector>

namespace pb {

	// Define Voxel struct
	template <typename ELEMENT>
	struct Voxel {
		// Constructor
		Voxel(ELEMENT const & e)
			: voxel_content(e) {}

		// Voxel content
		ELEMENT voxel_content;
	};

	template <typename ELEMENT>
	class VoxelGrid {
	public:
		// Constructor
		VoxelGrid(size_t const x_dim, size_t const y_dim, size_t const z_dim,
				  math::vec3f const & grid_min, math::vec3f const & grid_max);

		// Destructor
		~VoxelGrid();

		//
		// Preprocessing functions
		//
		
		// Remove all voxels that are compleatly surronded by other voxels that are not empty 
		void removeInteriorVoxels();

		// Set element value at a certain voxel
		void setElement(size_t const x, size_t const y, size_t const z, ELEMENT const & elem);

		// Get voxel
		Voxel<ELEMENT> const * getElement(size_t const x, size_t const y, size_t const z) const;

		// Get voxel center
		math::vec3f getVoxelCenter(size_t const x, size_t const y, size_t const z) const;

		// Get voxel grid dimensions
		size_t getGridDimX() const;
		size_t getGridDimY() const;
		size_t getGridDimZ() const;

		// Get iterator to first voxel
		typename std::vector<Voxel<ELEMENT> >::iterator voxelsBegin();
		typename std::vector<Voxel<ELEMENT> >::const_iterator voxelsBegin() const;

		// Get iterator to last voxel
		typename std::vector<Voxel<ELEMENT> >::iterator voxelsEnd();
		typename std::vector<Voxel<ELEMENT> >::const_iterator voxelsEnd() const;

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
		// List of voxels
		std::vector<Voxel<ELEMENT> *> voxels;
	};

	// VoxelGrid class methods implementation
	template <typename ELEMENT>
	VoxelGrid<ELEMENT>::VoxelGrid(size_t const x_dim, size_t const y_dim, size_t const z_dim,
								  math::vec3f const & grid_min, math::vec3f const & grid_max)
		: x_dim(x_dim), y_dim(y_dim), z_dim(z_dim),
		grid_min(grid_min), grid_max(grid_max),
		voxel_x((grid_max(0) - grid_min(0)) / static_cast<float>(x_dim)),
		voxel_y((grid_max(1) - grid_min(1)) / static_cast<float>(y_dim)),
		voxel_z((grid_max(2) - grid_min(2)) / static_cast<float>(z_dim)) {
		// Resize vector to old all voxels information
		voxels.resize(x_dim * y_dim * z_dim);
		// Set all voxels to null // TODO: CHECK IF WE NEED THIS
		for (auto it = voxels.begin(); it != voxels.end(); it++) {
			*it = nullptr;
		}
	}

	template <typename ELEMENT>
	VoxelGrid<ELEMENT>::~VoxelGrid() {
		// Free all voxels
		for (auto voxel = voxels.begin(); voxel != voxels.end(); voxel++) {
			if (*voxel) {
				delete (*voxel);
			}
		}
	}

	template <typename ELEMENT>
	void VoxelGrid<ELEMENT>::removeInteriorVoxels() {
		std::vector<size_t> delete_voxel;
		// Loop over all interior voxels
		for (size_t y = 1; y < y_dim - 1; y++) {
			for (size_t z = 1; z < z_dim - 1; z++) {
				for (size_t x = 1; x < x_dim - 1; x++) {
					// Check if surronding voxels are all non-null 
					if (voxels[x + 1 + x_dim * z + x_dim * z_dim * y] &&
						voxels[x - 1 + x_dim * z + x_dim * z_dim * y] &&
						voxels[x + x_dim * (z + 1) + x_dim * z_dim * y] &&
						voxels[x + x_dim * (z - 1) + x_dim * z_dim * y] &&
						voxels[x + x_dim * z + x_dim * z_dim * (y + 1)] &&
						voxels[x + x_dim * z + x_dim * z_dim * (y - 1)]) {
						// Set voxel index to be removed
						delete_voxel.push_back(x + x_dim * z + x_dim * z_dim * y);
					}
				}
			}
		}

		// Loop over all voxels marked and remove them
		for (auto it = delete_voxel.begin(); it != delete_voxel.end(); it++) {
			delete voxels[*it];
			voxels[*it] = nullptr;
		}
	}

	template <typename ELEMENT>
	void VoxelGrid<ELEMENT>::setElement(size_t const x, size_t const y, size_t const z, ELEMENT const & elem) {
#ifdef  _DEBUG
		assert(x >= 0 && x < x_dim);
		assert(y >= 0 && y < y_dim);
		assert(z >= 0 && z < z_dim);
#endif //  _DEBUG

		// Check if voxel as already been created
		if (voxels[x + x_dim * z + x_dim * z_dim * y]) {
			voxels[x + x_dim * z + x_dim * z_dim * y]->voxel_content = elem;
		} else {
			voxels[x + x_dim * z + x_dim * z_dim * y] = new Voxel<ELEMENT>(elem);
		}
	}

	template <typename ELEMENT>
	Voxel<ELEMENT> const * VoxelGrid<ELEMENT>::getElement(size_t const x, size_t const y, size_t const z) const {
#ifdef  _DEBUG
		assert(x >= 0 && x < x_dim);
		assert(y >= 0 && y < y_dim);
		assert(z >= 0 && z < z_dim);
#endif //  _DEBUG

		// Return element 
		return voxels[x + x_dim * z + x_dim * z_dim * y];
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

	template <typename ELEMENT>
	size_t VoxelGrid<ELEMENT>::getGridDimX() const {
		return x_dim;
	}

	template <typename ELEMENT>
	size_t VoxelGrid<ELEMENT>::getGridDimY() const {
		return y_dim;
	}

	template <typename ELEMENT>
	size_t VoxelGrid<ELEMENT>::getGridDimZ() const {
		return z_dim;
	}

	template <typename ELEMENT>
	typename std::vector<Voxel<ELEMENT> >::iterator VoxelGrid<ELEMENT>::voxelsBegin() {
		return voxels_content.begin();
	}

	template <typename ELEMENT>
	typename std::vector<Voxel<ELEMENT> >::const_iterator VoxelGrid<ELEMENT>::voxelsBegin() const {
		return voxels_content.begin();
	}

	template <typename ELEMENT>
	typename std::vector<Voxel<ELEMENT> >::iterator VoxelGrid<ELEMENT>::voxelsEnd() {
		return voxels_content.end();
	}

	template <typename ELEMENT>
	typename std::vector<Voxel<ELEMENT> >::const_iterator VoxelGrid<ELEMENT>::voxelsEnd() const {
		return voxels_content.end();
	}

} // pb namespace