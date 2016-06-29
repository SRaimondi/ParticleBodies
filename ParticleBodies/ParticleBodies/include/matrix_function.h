#pragma once

#include <cmath>
#include "math_utilities.h"
#include "matrix.h"

namespace pb {
	namespace math {

		// Meta dot struct
		template <size_t I, size_t SIZE>
		struct meta_dot {
			template <typename EXPR1, typename EXPR2, typename T>
			static inline T f(EXPR1 const & e1, EXPR2 const & e2, T const & x) {
				return (e1(I - 1) * e2(I - 1) + meta_dot<I + 1, SIZE>::f(e1, e2, x));
			}
		};

		template <size_t SIZE>
		struct meta_dot<SIZE, SIZE> {
			template <typename EXPR1, typename EXPR2, typename T>
			static inline T f(EXPR1 const & e1, EXPR2 const & e2, T const &) {
				return (e1(SIZE - 1) * e2(SIZE - 1));
			}
		};

		// Dot product
		template <typename T, size_t SIZE, typename EXPR1, typename EXPR2>
		T dotProduct(Matrix<T, SIZE, 1, EXPR1> const & v1, Matrix<T, SIZE, 1, EXPR2> const & v2) {
			return (meta_dot<1, SIZE>::f(v1, v2, T()));
		}

		// Square magnitude
		template <typename T, size_t SIZE, typename EXPR>
		T sqrMagnitude(Matrix<T, SIZE, 1, EXPR> const & v) {
			T sum = 0;
			for (size_t index = 0; index < SIZE; index++) {
				sum += v(index) * v(index);
			}

			return (sum);
		}

		// Magnitude 
		template <typename T, size_t SIZE, typename EXPR>
		T magnitude(Matrix<T, SIZE, 1, EXPR> const & v) {
			return (sqrt(sqrMagnitude(v)));
		}

		// Normalize vector matrix
		template <typename T, size_t SIZE, typename EXPR>
		Matrix<T, SIZE, 1> normalize(Matrix<T, SIZE, 1, EXPR> const & v) {
			T inv_mag = 1.f / magnitude(v);

			Matrix<T, SIZE, 1> result;

			for (size_t index = 0; index < SIZE; index++) {
				result(index) = v(index) * inv_mag;
			}

			return (result);
		}

		// Cross product
		template <typename T, typename EXPR1, typename EXPR2>
		Matrix<T, 3, 1> crossProduct(Matrix<T, 3, 1, EXPR1> const & v1, Matrix<T, 3, 1, EXPR2> const & v2) {
			return (Matrix<T, 3, 1>({ v1(1) * v2(2) - v1(2) * v2(1),
					v1(2) * v2(0) - v1(0) * v2(2),
					v1(0) * v2(1) - v1(1) * v2(0) }));
		}

		// Create orthogonal vector
		template <typename T, typename EXPR>
		Matrix<T, 3, 1> orthogonalVector(Matrix<T, 3, 1, EXPR> const & v) {
			if (fabsf(v(0)) < fabsf(v(1)) && fabsf(v(0)) < fabsf(v(2))) {
				return (Matrix<T, 3, 1>({ 0, -v(2), v(1) }));
			}
			else if (fabsf(v(1)) < fabsf(v(0)) && fabsf(v(1)) < fabsf(v(2))) {
				return (Matrix<T, 3, 1>({ -v(2), 0, v(0) }));
			}
			else {
				return (Matrix<T, 3, 1>({ -v(1), v(0), 0 }));
			}
		}

		// Transform location with matrix
		template <typename T, typename EXPR1, typename EXPR2>
		Matrix<T, 3, 1> transformLocation(Matrix<T, 4, 4, EXPR1> const & m, Matrix<T, 3, 1, EXPR2> const & v) {
			return Matrix<T, 3, 1>({ m(0, 0) * v(0) + m(0, 1) * v(1) + m(0, 2) * v(2) + m(0, 3),
								   m(1, 0) * v(0) + m(1, 1) * v(1) + m(1, 2) * v(2) + m(1, 3),
								   m(2, 0) * v(0) + m(2, 1) * v(1) + m(2, 2) * v(2) + m(2, 3) });
		}

		// Transform direction with matrix
		template <typename T, typename EXPR1, typename EXPR2>
		Matrix<T, 3, 1> transformDirection(Matrix<T, 4, 4, EXPR1> const & m, Matrix<T, 3, 1, EXPR2> const & v) {
			return Matrix<T, 3, 1>({ m(0, 0) * v(0) + m(0, 1) * v(1) + m(0, 2) * v(2),
								   m(1, 0) * v(0) + m(1, 1) * v(1) + m(1, 2) * v(2),
								   m(2, 0) * v(0) + m(2, 1) * v(1) + m(2, 2) * v(2) });
		}

		// Set matrix to identity
		template <typename T, size_t ROWS, size_t COLS, typename EXPR>
		void setToIdentity(Matrix<T, ROWS, COLS, EXPR> & m) {
			for (size_t r = 0; r < ROWS; r++) {
				for (size_t c = 0; c < COLS; c++) {
					if (r == c) {
						m(r, c) = 1.f;
					}
					else {
						m(r, c) = 0.f;
					}
				}
			}
		}

		// Transpose matrix
		template <typename T, size_t ROWS, size_t COLS, typename EXPR>
		Matrix<T, COLS, ROWS> transpose(Matrix<T, ROWS, COLS, EXPR> const & m) {
			Matrix<T, COLS, ROWS> transpose;

			for (size_t r = 0; r < ROWS; r++) {
				for (size_t c = 0; c < COLS; c++) {
					transpose(c, r) = m(r, c);
				}
			}

			return (transpose);
		}

		// Inverse matrix, only for 4x4 matrices
		template <typename T>
		T det3(T const a, T const b, T const c,
			   T const d, T const e, T const f,
			   T const g, T const h, T const i) {
			return (a * e * i + b * f * g + c * d * h - c * e * g - b * d * i - a * f * h);
		}

		// Matrix determinant, only for 4x4 matrices
		template <typename T, typename EXPR>
		T determinant(Matrix<T, 4, 4, EXPR> const & m) {
			T det;

			det = m(0, 0) * det3(m(1, 1), m(1, 2), m(1, 3),
								 m(2, 1), m(2, 2), m(2, 3),
								 m(3, 1), m(3, 2), m(3, 3));

			det -= m(0, 1) * det3(m(1, 0), m(1, 2), m(1, 3),
								  m(2, 0), m(2, 2), m(2, 3),
								  m(3, 0), m(3, 2), m(3, 3));

			det += m(0, 2) * det3(m(1, 0), m(1, 1), m(1, 3),
								  m(2, 0), m(2, 1), m(2, 3),
								  m(3, 0), m(3, 1), m(3, 3));

			det -= m(0, 3) * det3(m(1, 1), m(1, 2), m(1, 2),
								  m(2, 1), m(2, 2), m(2, 2),
								  m(3, 1), m(3, 2), m(3, 2));

			return (det);
		}

		// Compute cofactor matrix
		template <typename T, typename EXPR>
		Matrix<T, 4, 4> cofactor(Matrix<T, 4, 4, EXPR> const & m) {
			Matrix<T, 4, 4> result;

			/* Compute first row */
			result(0, 0) = det3(m(1, 1), m(1, 2), m(1, 3),
								m(2, 1), m(2, 2), m(2, 3),
								m(3, 1), m(3, 2), m(3, 3));
			result(0, 1) = -det3(m(1, 0), m(1, 2), m(1, 3),
								 m(2, 0), m(2, 2), m(2, 3),
								 m(3, 0), m(3, 2), m(3, 3));
			result(0, 2) = det3(m(1, 0), m(1, 1), m(1, 3),
								m(2, 0), m(2, 1), m(2, 3),
								m(3, 0), m(3, 1), m(3, 3));
			result(0, 3) = -det3(m(1, 0), m(1, 1), m(1, 2),
								 m(2, 0), m(2, 1), m(2, 2),
								 m(3, 0), m(3, 1), m(3, 2));

			/* Compute second row */
			result(1, 0) = -det3(m(0, 1), m(0, 2), m(0, 3),
								 m(2, 1), m(2, 2), m(2, 3),
								 m(3, 1), m(3, 2), m(3, 3));
			result(1, 1) = det3(m(0, 0), m(0, 2), m(0, 3),
								m(2, 0), m(2, 2), m(2, 3),
								m(3, 0), m(3, 2), m(3, 3));
			result(1, 2) = -det3(m(0, 0), m(0, 1), m(0, 3),
								 m(2, 0), m(2, 1), m(2, 3),
								 m(3, 0), m(3, 1), m(3, 3));
			result(1, 3) = det3(m(0, 0), m(0, 1), m(0, 2),
								m(2, 0), m(2, 1), m(2, 2),
								m(3, 0), m(3, 1), m(3, 2));

			/* Compute third row */
			result(2, 0) = det3(m(0, 1), m(0, 2), m(0, 3),
								m(1, 1), m(1, 2), m(1, 3),
								m(3, 1), m(3, 2), m(3, 3));
			result(2, 1) = -det3(m(0, 0), m(0, 2), m(0, 3),
								 m(1, 0), m(1, 2), m(1, 3),
								 m(3, 0), m(3, 2), m(3, 3));
			result(2, 2) = det3(m(0, 0), m(0, 1), m(0, 3),
								m(1, 0), m(1, 1), m(1, 3),
								m(3, 0), m(3, 1), m(3, 3));
			result(2, 3) = -det3(m(0, 0), m(0, 1), m(0, 2),
								 m(1, 0), m(1, 1), m(1, 2),
								 m(3, 0), m(3, 1), m(3, 2));

			/* Compute fourth row */
			result(3, 0) = -det3(m(0, 1), m(0, 2), m(0, 3),
								 m(1, 1), m(1, 2), m(1, 3),
								 m(2, 1), m(2, 2), m(2, 3));
			result(3, 1) = det3(m(0, 0), m(0, 2), m(0, 3),
								m(1, 0), m(1, 2), m(1, 3),
								m(2, 0), m(2, 2), m(2, 3));
			result(3, 2) = -det3(m(0, 0), m(0, 1), m(0, 3),
								 m(1, 0), m(1, 1), m(1, 3),
								 m(2, 0), m(2, 1), m(2, 3));
			result(3, 3) = det3(m(0, 0), m(0, 1), m(0, 2),
								m(1, 0), m(1, 1), m(1, 2),
								m(2, 0), m(2, 1), m(2, 2));

			return (result);
		}

		// Compute adjugate matrix
		template <typename T, typename EXPR>
		Matrix<T, 4, 4> adjugate(Matrix<T, 4, 4, EXPR> const & m) {
			return (transpose(cofactor(m)));
		}


		// Compute inverse matrix
		template <typename T, typename EXPR>
		Matrix<T, 4, 4> inverse(Matrix<T, 4, 4, EXPR> const & m) {
			T det = determinant(m);

			if (!approxEqual(det, 0.f)) {
				return ((1.f / det) * adjugate(m));
			}
			else {
				return (Matrix<T, 4, 4>());
			}
		}

		// Create translation matrix
		template <typename T>
		Matrix<T, 4, 4> translationMatrix(T const x, T const y, T const z) {
			return (Matrix<T, 4, 4>({ 1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1 }));
		}

		template <typename T, typename EXPR>
		Matrix<T, 4, 4> translationMatrix(Matrix<T, 3, 1, EXPR> const & t) {
			return (Matrix<T, 4, 4>({ 1, 0, 0, t(0),
					0, 1, 0, t(1),
					0, 0, 1, t(2),
					0, 0, 0, 1 }));
		}

		// Create scaling matrix
		template <typename T>
		Matrix<T, 4, 4> scalingMatrix(T const x, T const y, T const z) {
			return (Matrix<T, 4, 4>({ x, 0, 0, 0,
					0, y, 0, 0,
					0, 0, z, 0,
					0, 0, 0, 1 }));
		}

		template <typename T, typename EXPR>
		Matrix<T, 4, 4> scalingMatrix(Matrix<T, 3, 1, EXPR> const & s) {
			return (Matrix<T, 4, 4>({ s(0), 0, 0, 0,
					0, s(1), 0, 0,
					0, 0, s(2), 0,
					0, 0, 0, 1 }));
		}


		// Create rotation matrix
		template <typename T>
		Matrix<T, 4, 4> rotateXMatrix(T const deg) {
			T s = sinf(degToRad(deg));
			T c = cosf(degToRad(deg));

			return (Matrix<T, 4, 4>({ 1, 0, 0, 0,
					0, c, -s, 0,
					0, s, c, 0,
					0, 0, 0, 1 }));
		}

		template <typename T>
		Matrix<T, 4, 4> rotateYMatrix(T const deg) {
			T s = sinf(degToRad(deg));
			T c = cosf(degToRad(deg));

			return (Matrix<T, 4, 4>({ c, 0, s, 0,
					0, 1, 0, 0,
					-s, 0, c, 0,
					0, 0, 0, 1 }));
		}

		template <typename T>
		Matrix<T, 4, 4> rotateZMatrix(T const deg) {
			T s = sinf(degToRad(deg));
			T c = cosf(degToRad(deg));

			return (Matrix<T, 4, 4>({ c, -s, 0, 0,
					s, c, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 }));
		}

		template <typename T, typename EXPR>
		Matrix<T, 4, 4> rotateMatrix(Matrix<T, 3, 1, EXPR> const & axis, const T deg) {
			Matrix<T, 3, 1> normalized_axis = normalize(axis);

			T x = normalized_axis(0);
			T y = normalized_axis(1);
			T z = normalized_axis(2);
			T c = cosf(degToRad(deg));
			T s = sinf(degToRad(deg));
			T t = 1.f - c;

			return (Matrix<T, 4, 4>({ c * x * x * t, t * x * y - z * s, t * x * z + y * s, 0,
					t * x * y + z * s, c + t * y * y, t * y * z - x * s, 0,
					t * x * z - y * s, t * y * z + x * s, c + t * z * z, 0,
					0, 0, 0, 1 }));
		}

		// Create view matrix
		template <typename T, typename EXPR1, typename EXPR2, typename EXPR3>
		Matrix<T, 4, 4> lookatMatrix(Matrix<T, 3, 1, EXPR1> const & eye, Matrix<T, 3, 1, EXPR2> const& at,
									 Matrix<T, 3, 1, EXPR3> const & up) {
			Matrix<T, 3, 1> w = normalize(eye - at);
			Matrix<T, 3, 1> u = normalize(crossProduct(up, w));
			Matrix<T, 3, 1> v = crossProduct(w, u);

			return (Matrix<T, 4, 4>({ u(0), v(0), w(0), eye(0),
					u(1), v(1), w(1), eye(1),
					u(2), v(2), w(2), eye(2),
					0, 0, 0, 1 }));
		}

	} // math namespace
} // pb namespace