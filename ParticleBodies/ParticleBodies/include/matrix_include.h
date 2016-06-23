#pragma once

#include "matrix.h"
#include "matrix_operators.h"
#include "matrix_function.h"

namespace pb {
	namespace math {

		// Define common types
		typedef Matrix<float, 4, 4> mat4x4f;
		typedef Matrix<double, 4, 4> mat4x4d;

		typedef Matrix<float, 4, 1> vec4f;
		typedef Matrix<double, 4, 1> vec4d;

		typedef Matrix<float, 3, 1> vec3f;
		typedef Matrix<double, 3, 1> vec3d;

		typedef Matrix<float, 2, 1> vec2f;
		typedef Matrix<double, 2, 1> vec2d;

	} // math namespace
} // pb namespace