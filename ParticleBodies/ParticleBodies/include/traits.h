#pragma once

namespace pb {

	namespace math {
		template <typename T> class Scalar;
	}

	// Base template
	template <typename T>
	class ExprTraits {
	public:
		typedef T const & ExprReference;     // Refer as constant reference
	};

	// Specialization for scalars
	template <typename T>
	class ExprTraits<pb::math::Scalar<T> > {
	public:
		typedef pb::math::Scalar<T> ExprReference;  // Refer as value
	};

} // pb namespace
