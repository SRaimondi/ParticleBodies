#pragma once

// Include traits helper class
#include "scalar.h"
#include "traits.h"
#include "matrix.h"

namespace pb {
	namespace math {

		// Matrix sum
		template <typename T, typename OP1, typename OP2>
		class OPMatrixAdd {
		private:
			typename ExprTraits<OP1>::ExprReference op1;
			typename ExprTraits<OP2>::ExprReference op2;

		public:
			// Constructor
			OPMatrixAdd(OP1 const & o1, OP2 const & o2)
				: op1(o1), op2(o2) {}

			// Access operator returns sum
			T operator() (size_t const i, size_t const j) const {
				return (op1(i, j) + op2(i, j));
			}
		};

		// Matrix sub
		template <typename T, typename OP1, typename OP2>
		class OPMatrixSub {
		private:
			typename ExprTraits<OP1>::ExprReference op1;
			typename ExprTraits<OP2>::ExprReference op2;

		public:
			// Constructor
			OPMatrixSub(OP1 const & o1, OP2 const & o2)
				: op1(o1), op2(o2) {}

			// Access operator returns sub
			T operator() (size_t const i, size_t const j) const {
				return (op1(i, j) - op2(i, j));
			}
		};

		// Matrix - Matrix Multiplication
		template <typename T, typename OP1, typename OP2, size_t SIZE = 0>
		class OPMatrixMult {
		private:
			typename ExprTraits<OP1>::ExprReference op1;
			typename ExprTraits<OP2>::ExprReference op2;

		public:
			// Constructor
			OPMatrixMult(OP1 const & o1, OP2 const & o2)
				: op1(o1), op2(o2) {}

			// Access operator returns multiplication
			T operator() (size_t const i, size_t const j) const {
				T sum = 0;

				for (size_t k = 0; k < SIZE; k++) {
					sum += op1(i, k) * op2(k, j);
				}

				return (sum);
			}
		};

		// Specialization for scalars
		template <typename T, typename OP>
		class OPMatrixMult<T, Scalar<T>, OP> {
		private:
			typename ExprTraits<Scalar<T> >::ExprReference s;
			typename ExprTraits<OP>::ExprReference op;

		public:
			// Constructor
			OPMatrixMult(Scalar<T> const & s, OP const & op)
				: s(s), op(op) {}

			// Access operator returns element value
			T operator() (size_t const i, size_t const j) const {
				return (s(i, j) * op(i, j));
			}
		};

		// Negate Matrix
		template <typename T, typename OP>
		class OPMatrixNeg {
		private:
			typename ExprTraits<OP>::ExprReference op;

		public:
			// Constructor initializes reference to operand
			OPMatrixNeg(OP const & o)
				: op(o) {}

			// Compute negation when value requested
			T operator() (size_t const i, size_t const j) const {
				return (-op(i, j));
			}
		};

		// Define the operators returning the lightweight objects representing operations

		// Matrix addition
		template <typename T, size_t ROWS, size_t COLS, typename EXPR1, typename EXPR2>
		Matrix<T, ROWS, COLS, OPMatrixAdd<T, EXPR1, EXPR2> >
			operator+ (Matrix<T, ROWS, COLS, EXPR1> const & e1, Matrix<T, ROWS, COLS, EXPR2> const & e2) {
			return (Matrix<T, ROWS, COLS, OPMatrixAdd<T, EXPR1, EXPR2> >
				(OPMatrixAdd<T, EXPR1, EXPR2>(e1.expr(), e2.expr())));
		}

		// Matrix subtraction
		template <typename T, size_t ROWS, size_t COLS, typename EXPR1, typename EXPR2>
		Matrix<T, ROWS, COLS, OPMatrixSub<T, EXPR1, EXPR2> >
			operator- (Matrix<T, ROWS, COLS, EXPR1> const & e1, Matrix<T, ROWS, COLS, EXPR2> const & e2) {
			return (Matrix<T, ROWS, COLS, OPMatrixSub<T, EXPR1, EXPR2> >
				(OPMatrixSub<T, EXPR1, EXPR2>(e1.expr(), e2.expr())));
		}

		// Scalar - Matrix multiplication
		template <typename T, size_t ROWS, size_t COLS, typename EXPR>
		Matrix<T, ROWS, COLS, OPMatrixMult<T, Scalar<T>, EXPR> >
			operator* (T const & s, Matrix<T, ROWS, COLS, EXPR> const & e) {
			return (Matrix<T, ROWS, COLS, OPMatrixMult<T, Scalar<T>, EXPR> >
				(OPMatrixMult<T, Scalar<T>, EXPR>(Scalar<T>(s), e.expr())));
		}

		template <typename T, size_t ROWS, size_t COLS, typename EXPR>
		Matrix<T, ROWS, COLS, OPMatrixMult<T, Scalar<T>, EXPR> >
			operator* (Matrix<T, ROWS, COLS, EXPR> const & e, T const & s) {
			return (Matrix<T, ROWS, COLS, OPMatrixMult<T, Scalar<T>, EXPR> >
				(OPMatrixMult<T, Scalar<T>, EXPR>(Scalar<T>(s), e.expr())));
		}

		// Matrix - Scalar division
		template <typename T, size_t ROWS, size_t COLS, typename EXPR>
		Matrix<T, ROWS, COLS, OPMatrixMult<T, Scalar<T>, EXPR> >
			operator/ (Matrix<T, ROWS, COLS, EXPR> const & e, T const & s) {
			return (Matrix<T, ROWS, COLS, OPMatrixMult<T, Scalar<T>, EXPR> >
				(OPMatrixMult<T, Scalar<T>, EXPR>(Scalar<T>(T(1) / s), e.expr())));
		}

		// Matrix - matrix multiplication
		template <typename T, size_t ROWS, size_t SIZE, size_t COLS, typename EXPR1, typename EXPR2>
		Matrix<T, ROWS, COLS, OPMatrixMult<T, EXPR1, EXPR2, SIZE> >
			operator* (Matrix<T, ROWS, SIZE, EXPR1> const & e1, Matrix<T, SIZE, COLS, EXPR2> const & e2) {
			return (Matrix<T, ROWS, COLS, OPMatrixMult<T, EXPR1, EXPR2, SIZE> >
				(OPMatrixMult<T, EXPR1, EXPR2, SIZE>(e1.expr(), e2.expr())));
		}

		// Negation of Matrix
		template <typename T, size_t ROWS, size_t COLS, typename EXPR>
		Matrix<T, ROWS, COLS, OPMatrixNeg<T, EXPR> >
			operator- (Matrix<T, ROWS, COLS, EXPR> const & e) {
			return (Matrix<T, ROWS, COLS, OPMatrixNeg<T, EXPR> >
				(OPMatrixNeg<T, EXPR>(e.expr())));
		}

	} // math namespace
} // pb namespace