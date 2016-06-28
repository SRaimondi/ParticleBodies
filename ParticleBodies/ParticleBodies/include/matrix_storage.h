#pragma once

#include <cstring>
#include <initializer_list>
#include <cassert>

namespace pb {
	namespace math {

		template <typename T, size_t ROWS, size_t COLS>
		class MatrixStorage {
		public:
			// Constructors
			explicit MatrixStorage() {
				e = new T[ROWS * COLS];

				// Set all values to zero
				for (size_t i = 0; i < ROWS * COLS; i++) {
					e[i] = T(0);
				}
			}

			MatrixStorage(std::initializer_list<std::initializer_list<T> > const & args) {
#ifdef _DEBUG
				assert(args.size() == ROWS && args.begin()->size() == COLS);
#endif
				// Allocate memory
				e = new T[ROWS * COLS];

				size_t row = 0;
				size_t col = 0;

				for (auto row_it = args.begin(); row_it != args.end(); row_it++) {
					for (auto col_it = row_it->begin(); col_it != row_it->end(); col_it++) {
						e[row * COLS + col] = *col_it;
						col++;
					}
					row++;
				}
			}

			MatrixStorage(std::initializer_list<T> const & args) {
#ifdef _DEBUG
				assert(args.size() == ROWS * COLS);
#endif
				// Allocate memory
				e = new T[ROWS * COLS];

				size_t row = 0;
				size_t col = 0;

				for (auto elem_it = args.begin(); elem_it != args.end(); elem_it++) {
					e[row * COLS + col] = *elem_it;
					col++;
					if (col == COLS) {
						col = 0;
						row++;
					}
				}
			}

			// Destructor
			~MatrixStorage() {
				delete[] e;
			}

			// Copy constructor
			MatrixStorage(MatrixStorage const & orig) {
				// Allocate memory
				e = new T[ROWS * COLS];

				for (size_t r = 0; r < ROWS; r++) {
					for (size_t c = 0; c < COLS; c++) {
						e[r * COLS + c] = orig.e[r * COLS + c];
					}
				}
			}

			// Move constructor
			MatrixStorage(MatrixStorage && src) {
				// Set pointer to src memory
				e = src.e;

				// Set src memory pointer to null
				src.e = nullptr;
			}

			// Assignment operator
			MatrixStorage & operator= (MatrixStorage const & other) {
				if (&other != this) {
					for (size_t r = 0; r < ROWS; r++) {
						for (size_t c = 0; c < COLS; c++) {
							e[r * COLS + c] = other.e[r * COLS + c];
						}
					}
				}

				return (*this);
			}

			// Move assignment operator
			MatrixStorage& operator= (MatrixStorage && rhs) {
				if (&rhs != this) {
					// Free memory for this matrix
					delete[] e;

					// Set pointer to src memory
					e = rhs.e;

					// Set rhs memory pointer to null
					rhs.e = nullptr;
				}

				return (*this);
			}

			// Index operator
			T operator() (size_t const row, size_t const col) const {
#ifdef _DEBUG
				assert(row >= 0 && row < ROWS);
				assert(col >= 0 && col < COLS);
#endif

				return e[row * COLS + col];
			}

			T & operator() (size_t const row, size_t const col) {
#ifdef _DEBUG
				assert(row >= 0 && row < ROWS);
				assert(col >= 0 && col < COLS);
#endif

				return e[row * COLS + col];
			}

		private:
			// Matrix data
			T* e;
		};

		// Column vector specialization
		template <typename T, size_t ROWS>
		class MatrixStorage<T, ROWS, 1> {
		public:
			// Constructors
			explicit MatrixStorage() {
				e = new T[ROWS];
			}

			MatrixStorage(std::initializer_list<T> const & args) {
#ifdef _DEBUG
				assert(args.size() == ROWS);
#endif
				// Allocate memory
				e = new T[ROWS];

				size_t row = 0;
				for (auto elem_it = args.begin(); elem_it != args.end(); elem_it++) {
					e[row] = *elem_it;
					row++;
				}
			}

			// Destructor
			~MatrixStorage() {
				delete[] e;
			}

			// Copy constructor
			MatrixStorage(MatrixStorage const & orig) {
				// Allocate memory
				e = new T[ROWS];

				for (size_t r = 0; r < ROWS; r++) {
					e[r] = orig.e[r];
				}
			}

			// Move constructor
			MatrixStorage(MatrixStorage && src) {
				// Set pointer to src memory
				e = src.e;

				// Set src memory pointer to null
				src.e = nullptr;
			}

			// Assignment operator
			MatrixStorage & operator= (MatrixStorage const & other) {
				if (&other != this) {
					for (size_t r = 0; r < ROWS; r++) {
						e[r] = other.e[r];
					}
				}

				return (*this);
			}

			// Move assignment operator
			MatrixStorage& operator= (MatrixStorage && rhs) {
				if (&rhs != this) {
					// Free memory for this matrix
					delete[] e;

					// Set pointer to src memory
					e = rhs.e;

					// Set rhs memory pointer to null
					rhs.e = nullptr;
				}

				return (*this);
			}

			// Index operator
			T operator() (size_t const row, size_t const) const {
#ifdef _DEBUG
				assert(row >= 0 && row < ROWS);
#endif

				return e[row];
			}

			T & operator() (size_t const row, size_t const) {
#ifdef _DEBUG
				assert(row >= 0 && row < ROWS);
#endif

				return e[row];
			}

		private:
			// Matrix data
			T* e;
		};

	} // math namespace
} // pb namespace