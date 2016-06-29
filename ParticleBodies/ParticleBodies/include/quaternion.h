#pragma once

#include "matrix_include.h"

namespace pb {
	namespace math {

		// Forward declare Quaternion class
		template <typename T> class Quaternion;

		// Forward declare friend function

		// Magnitude of quaternion
		template <typename T>
		T magnitude(Quaternion<T> const & q);

		// Extract rotation angle
		template <typename T>
		T extractAngle(Quaternion<T> const & q);

		// Extract rotation axis
		template <typename T>
		Matrix<T, 3, 1> extractAxis(Quaternion<T> const & q);

		// Create rotation matrix
		template <typename T>
		Matrix<T, 4, 4> createRotationMatrix(Quaternion<T> const & q);

		// Create rotation matrix 3x3
		template <typename T>
		Matrix<T, 3, 3> createMatrix(Quaternion<T> const & q);

		// Norm of the quaterion
		template <typename T>
		T norm(Quaternion<T> const & q);

		// Normalize quaternion
		template <typename T>
		Quaternion<T> normalize(Quaternion<T> const & q);

		// Conjugate quaternion
		template <typename T>
		Quaternion<T> conjugate(Quaternion<T> const & q);

		// Transform vector by quaternion
		template <typename T>
		Matrix<T, 3, 1> transformVectorByQuaternion(Quaternion<T> const & q, Matrix<T, 3, 1> const & v);

		// Define quaternion class
		template <typename T>
		class Quaternion {
		public:
			// Constructor
			Quaternion()
				: real(0.0), immaginary() {}

			Quaternion(T const & r, Matrix<T, 3, 1> const & i)
				: real(r), immaginary(i) {}

			Quaternion(T const & r, T const & x, T const & y, T const & z)
				: real(r), immaginary({ x, y, z }) {}

			Quaternion(Quaternion const & other)
				: real(other.real), immaginary(other.immaginary) {}

			// Quaternion product
			Quaternion operator*(Quaternion const & other) const {
				Quaternion result;
				// Compute real part
				result.real = real * other.real - dotProduct(immaginary, other.immaginary);
				// Compute immaginary part
				result.immaginary = real * other.immaginary + other.real * immaginary + crossProduct(immaginary, other.immaginary);

				return result;
			}

			// Scale quaternion
			Quaternion operator*(T const & t) const {
				return Quaternion(real * t,
								  immaginary(0) * t,
								  immaginary(1) * t,
								  immaginary(2) * t);
			}

			// Divide quaternion
			Quaternion operator/(T const & t) const {
				return Quaternion(real / t,
								  immaginary(0) / t,
								  immaginary(1) / t,
								  immaginary(2) / t);
			}

			// Sum
			Quaternion operator+(Quaternion const & other) const {
				return Quaternion(real + other.real, immaginary + other.immaginary);
			}

			// Subtraction
			Quaternion operator-(Quaternion const & other) const {
				return Quaternion(real - other.real, immaginary - other.immaginary);
			}

			// Get real part
			T const & getReal() const {
				return real;
			}

			T & getReal() {
				return real;
			}

			// Get immaginary part
			Matrix<T, 3, 1> const & getImmaginary() const {
				return immaginary;
			}

			Matrix<T, 3, 1> & getImmaginary() {
				return immaginary;
			}

			// Friend functions
			template <typename T>
			friend T magnitude(Quaternion<T> const & q);

			template <typename T>
			friend T extractAngle(Quaternion<T> const & q);

			template <typename T>
			friend Matrix<T, 3, 1> extractAxis(Quaternion<T> const & q);

			template <typename T>
			friend Matrix<T, 4, 4> createRotationMatrix(Quaternion<T> const & q);

			template <typename T>
			friend Matrix<T, 3, 3> createMatrix(Quaternion<T> const & q);

			template <typename T>
			friend T norm(Quaternion<T> const & q);

			template <typename T>
			friend Quaternion<T> normalize(Quaternion<T> const & q);

			template <typename T>
			friend Quaternion<T> conjugate(Quaternion<T> const & q);

			template <typename T>
			friend Matrix<T, 3, 1> transformVectorByQuaternion(Quaternion<T> const & q, Matrix<T, 3, 1> const & v);

		private:
			// Real part
			T real;
			// Immaginary part
			Matrix<T, 3, 1> immaginary;
		};
	
		// Function implementation
		template <typename T>
		T magnitude(Quaternion<T> const & q) {
			return sqrt(q.real * q.real + sqrMagnitude(q.immaginary));
		}

		template <typename T>
		T extractAngle(Quaternion<T> const & q) {
			return 2.f * acos(q.real);
		}

		template <typename T>
		Matrix<T, 3, 1> extractAxis(Quaternion<T> const & q) {
			return q.immaginary / sin(0.5 * extractAngle(q));
		}

		template <typename T>
		Matrix<T, 4, 4> createRotationMatrix(Quaternion<T> const & q) {
			Matrix<T, 4, 4> result;

			T s = q.real;
			T x = q.immaginary(0);
			T y = q.immaginary(1);
			T z = q.immaginary(2);

			// Assign matrix entries
			// First row
			result(0, 0) = 1.0 - 2.0 * y * y - 2.0 * z * z;
			result(0, 1) = 2.0 * x * y - 2.0 * s * z;
			result(0, 2) = 2.0 * x * z + 2.0 * s * y;
			result(0, 3) = 0.0;

			// Second row
			result(1, 0) = 2.0 * x * y + 2.0 * s * z;
			result(1, 1) = 1.0 - 2.0 * x * x - 2.0 * z * z;
			result(1, 2) = 2.0 * y * z - 2.0 * s * x;
			result(1, 3) = 0.0;

			// Third row
			result(2, 0) = 2.0 * x * z - 2.0 * s * y;
			result(2, 1) = 2.0 * y * z + 2.0 * s * x;
			result(2, 2) = 1.0 - 2.0 * x * x - 2.0 * y * y;
			result(2, 3) = 0.0;


			// Fourth row
			result(3, 0) = 0.0;
			result(3, 1) = 0.0;
			result(3, 2) = 0.0;
			result(3, 3) = 1.0;

			return result;
		}

		template <typename T>
		Matrix<T, 3, 3> createMatrix(Quaternion<T> const & q) {
			Matrix<T, 3, 3> result;

			T s = q.real;
			T x = q.immaginary(0);
			T y = q.immaginary(1);
			T z = q.immaginary(2);

			// Assign matrix entries
			// First row
			result(0, 0) = 1.0 - 2.0 * y * y - 2.0 * z * z;
			result(0, 1) = 2.0 * x * y - 2.0 * s * z;
			result(0, 2) = 2.0 * x * z + 2.0 * s * y;

			// Second row
			result(1, 0) = 2.0 * x * y + 2.0 * s * z;
			result(1, 1) = 1.0 - 2.0 * x * x - 2.0 * z * z;
			result(1, 2) = 2.0 * y * z - 2.0 * s * x;

			// Third row
			result(2, 0) = 2.0 * x * z - 2.0 * s * y;
			result(2, 1) = 2.0 * y * z + 2.0 * s * x;
			result(2, 2) = 1.0 - 2.0 * x * x - 2.0 * y * y;

			return result;
		}

		// Create Quaternion from angle and axis
		template <typename T>
		Quaternion<T> quaternionFromAngleAxis(T const & angle, Matrix<T, 3, 1> const & axis) {
			return Quaternion<T>(static_cast<float>(cos(degToRad(angle / 2.0))),
								 static_cast<float>(sin(degToRad(angle / 2.0))) * axis);
		}

		template <typename T>
		T norm(Quaternion<T> const & q) {
			return sqrt(q.real * q.real + sqrMagnitude(q.immaginary));
		}

		template <typename T>
		Quaternion<T> normalize(Quaternion<T> const & q) {
			T norm = norm(q);

			return Quaternion<T>(q.real / norm, q.immaginary / norm);
		}

		template <typename T>
		Quaternion<T> conjugate(Quaternion<T> const & q) {
			return Quaternion<T>(q.real, -q.immaginary);
		}

		template <typename T>
		Matrix<T, 3, 1> transformVectorByQuaternion(Quaternion<T> const & q, Matrix<T, 3, 1> const & v) {
			// Transform v as quaternion by q * v * q_conjuvate
			return (q * Quaternion<T>(0.0, v) * conjugate(q)).immaginary;
		}

		// Define quaternion types
		typedef Quaternion<float> quaternionf;
		typedef Quaternion<double> quaterniond;

	} // math namespace
} // pb namespace