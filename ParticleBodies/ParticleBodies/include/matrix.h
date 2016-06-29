#pragma once

#include "matrix_storage.h"

namespace pb {
	namespace math {

		template <typename T, size_t ROWS, size_t COLS, typename EXPR = MatrixStorage<T, ROWS, COLS> >
		class Matrix {
		public:
			// Empty Matrix
			explicit Matrix()
				: expression() {}

			Matrix(std::initializer_list<std::initializer_list<T> > const & args)
				: expression(args) {}

			// Row-major single list constructor
			Matrix(std::initializer_list<T> const & args)
				: expression(args) {}

			// Create matrix form expression
			Matrix(EXPR const & expr)
				: expression(expr) {}

			// Generic copy with evaluation
			template <typename T2, typename EXPR2>
			Matrix(Matrix<T2, ROWS, COLS, EXPR2> const & other)
				: expression() {
				for (size_t r = 0; r < ROWS; r++) {
					for (size_t c = 0; c < COLS; c++) {
						expression(r, c) = other(r, c);
					}
				}
			}

			// Assignment operator
			template <typename T2, typename EXPR2>
			Matrix & operator= (Matrix<T2, ROWS, COLS, EXPR> const & other) {
				for (size_t r = 0; r < ROWS; r++) {
					for (size_t c = 0; c < COLS; c++) {
						expression(r, c) = other(r, c);
					}
				}

				return (*this);
			}

			// Index operator
			T operator() (size_t const row, size_t const col) const {
#ifdef _DEBUG
				assert(row >= 0 && row < ROWS);
				assert(col >= 0 && col < COLS);
#endif

				return expression(row, col);
			}

			T & operator() (size_t const row, size_t const col) {
#ifdef _DEBUG
				assert(row >= 0 && row < ROWS);
				assert(col >= 0 && col < COLS);
#endif

				return expression(row, col);
			}

			// Return the expression
			EXPR const & expr() const {
				return expression;
			}

			EXPR & expr() {
				return expression;
			}

		private:
			// Expression content
			EXPR expression;
		};

		// Specialization for single dimension vector
		template <typename T, size_t ROWS, typename EXPR>
		class Matrix<T, ROWS, 1, EXPR> {
		public:
			// Empty Matrix
			explicit Matrix()
				: expression() {}

			Matrix(std::initializer_list<std::initializer_list<T> > const & args)
				: expression(args) {}

			// Row-major single list constructor
			Matrix(std::initializer_list<T> const & args)
				: expression(args) {}

			// Create matrix form expression
			Matrix(EXPR const & expr)
				: expression(expr) {}

			// Generic copy with evaluation
			template <typename T2, typename EXPR2>
			Matrix(Matrix<T2, ROWS, 1, EXPR2> const & other)
				: expression() {
				for (size_t r = 0; r < ROWS; r++) {
					expression(r, 0) = other(r);
				}
			}

			// Assignment operator
			template <typename T2, typename EXPR2>
			Matrix & operator= (Matrix<T2, ROWS, 1, EXPR> const & other) {
				for (size_t r = 0; r < ROWS; r++) {
					expression(r, 0) = other(r);
				}

				return (*this);
			}

			// Index operator
			T operator() (size_t const row) const {
#ifdef _DEBUG
				assert(row >= 0 && row < ROWS);
#endif

				return expression(row, 0);
			}

			T & operator() (size_t const row) {
#ifdef _DEBUG
				assert(row >= 0 && row < ROWS);
#endif

				return (expression(row, 0));
			}

			// Return the expression
			EXPR const & expr() const {
				return (expression);
			}

			EXPR & expr() {
				return (expression);
			}

		private:
			// Expression content
			EXPR expression;
		};

	} // math namespace
} // pb namespace
