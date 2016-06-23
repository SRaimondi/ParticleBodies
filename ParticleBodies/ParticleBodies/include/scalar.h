#pragma once

namespace pb {
	namespace math {

		template <typename T>
		class Scalar {
		private:
			// Value of the scalar
			T const & s;

		public:
			// Constructor
			Scalar(T const & v)
				: s(v) {}

			// Index operator
			T operator() (size_t const) const {
				return (s);
			}

			T operator() (size_t const, size_t const) const {
				return (s);
			}
		};

	} // math namespace
} // pb namespace
