#pragma once

#include <cmath>
#include "constants.h"

namespace pb {
	namespace math {

		// Approx equal function
		template <typename T>
		bool approxEqual(T const & v1, T const & v2, T const & tol = EPS) {
			return ((fabsf(v1 - v2)) < tol);
		}

		// Degree to radians
		template <typename T>
		T degToRad(T const & deg) {
			return ((PI / 180.f) * deg);
		}

		// Radians to degree
		template <typename T>
		T radToDeg(T const & rad) {
			return ((180.f / PI) * rad);
		}

		// Clamp value
		template <typename T>
		T clamp(T const & val, T const & min, T const & max) {
			return (val < min ? min : (val > max ? max : val));
		}

		// Max and minimum
		template <typename T>
		T max(T const & a, T const & b) {
			return (a > b ? a : b);
		}

		template <typename T>
		T min(T const & a, T const & b) {
			return (a < b ? a : b);
		}

	} // math namespace
} // pb namespace
