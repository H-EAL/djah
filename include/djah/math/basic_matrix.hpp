#ifndef DJAH_MATH_BASIC_MATRIX_HPP
#define DJAH_MATH_BASIC_MATRIX_HPP

#include <algorithm>
#include <iostream>

#include "detail/vector_base.hpp"

namespace djah { namespace math {

	template<size_t SIZE, typename T>
	class basic_matrix : public detail::vector_base< SIZE*SIZE,T >
	{
	public:

		// A row type
		typedef detail::vector_base<SIZE,T> row_t;
		// A col type
		typedef row_t						col_t;

		// Init with zeros
		basic_matrix();
		// Init with a 1D array
		basic_matrix(const T (&array)[SIZE*SIZE]);
		// Init with a 2D array
		basic_matrix(const T (&array)[SIZE][SIZE]);

		// Retrieve a specific element
		template<size_t I, size_t J>
		T& m();
		template<size_t I, size_t J>
		const T& m() const;

		// Set to identity
		void identity();
		// Transpose this matrix
		void transpose();
		// Replace the current matrix by its adjugate
		void adjugate();
		// Invert this matrix
		void invert();
		// Compute determinant
		T getDeterminant() const;

		// Retrieve a row
		row_t row(size_t i) const;
		// Retrieve a col
		col_t col(size_t j) const;

		// Negate
		const basic_matrix<SIZE,T> operator -() const;

		// Addition
		basic_matrix<SIZE,T>& operator +=(const basic_matrix<SIZE,T> &rhs);
		// Subtraction
		basic_matrix<SIZE,T>& operator -=(const basic_matrix<SIZE,T> &rhs);
		// Matrix product
		basic_matrix<SIZE,T>& operator *=(const basic_matrix<SIZE,T> &rhs);
		// Scalar product
		basic_matrix<SIZE,T>& operator *=(T rhs);
		// Scalar division
		basic_matrix<SIZE,T>& operator /=(T rhs);


	protected:

		// Address of an element at a specified location
		T* at(size_t i, size_t j);
		const T* at(size_t i, size_t j) const;

		// Feed output stream with data
		virtual void print(std::ostream &out) const;
	};


	// Addition
	template<size_t SIZE, typename T>
	const basic_matrix<SIZE,T> operator +(const basic_matrix<SIZE,T> &lhs, const basic_matrix<SIZE,T> &rhs);
	// Subtraction
	template<size_t SIZE, typename T>
	const basic_matrix<SIZE,T> operator -(const basic_matrix<SIZE,T> &lhs, const basic_matrix<SIZE,T> &rhs);
	// Scalar product
	template<size_t SIZE, typename T>
	const basic_matrix<SIZE,T> operator *(const basic_matrix<SIZE,T> &lhs, T rhs);
	template<size_t SIZE, typename T>
	const basic_matrix<SIZE,T> operator *(T lhs, const basic_matrix<SIZE,T> &rhs);
	// Scalar division
	template<size_t SIZE, typename T>
	const basic_matrix<SIZE,T> operator /(const basic_matrix<SIZE,T> &lhs, T rhs);

	// Matrix product
	template<size_t SIZE, typename T>
	const basic_matrix<SIZE,T> operator *(const basic_matrix<SIZE,T> &lhs, const basic_matrix<SIZE,T> &rhs);

} /*math*/ } /*djah*/


#include "basic_matrix.inl"


#endif /* DJAH_MATH_BASIC_MATRIX_HPP */