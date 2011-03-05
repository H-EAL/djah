#ifndef DJAH_MATH_BASIC_MATRIX_HPP
#define DJAH_MATH_BASIC_MATRIX_HPP

#include <algorithm>
#include <iostream>
#include <cstring>
#include "vector.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	struct matrix_base
	{
		matrix_base() { memset(data, 0, N*N*sizeof(T)); }
		T data[N*N];
	};
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	class matrix
		: public matrix_base<N,T>
	{
	public:
		
		static const int size = N;
		typedef T value_type;

		// A row/col type
		typedef vector<N,T> row_t;
		typedef vector<N,T> col_t;

		// Default constructor -> null matrix ?
		matrix();
		
		// Constructors from values, only the needed one is actually created
		matrix(T _11, T _21,  T _12, T _22)
			: matrix_base<N,T>(_11,_21,  _12,_22) {}
		matrix(T _11, T _21, T _31,  T _12, T _22, T _32,  T _13, T _23, T _33)
			: matrix_base<N,T>(_11,_21,_31,  _12,_22,_32,  _13,_23,_33) {}
		matrix(T _11, T _21, T _31, T _41,  T _12, T _22, T _32, T _42,  T _13, T _23, T _33, T _43,  T _14, T _24, T _34, T _44)
			: matrix_base<N,T>(_11,_21,_31,_41,  _12,_22,_32,_42,  _13,_23,_33,_43,  _14,_24,_34,_44) {}
		
		// Constructors from array
		matrix(const T (&array)[N*N]);
		matrix(const T (&array)[N][N]);
		
		// Matrix math
		// Those functions change the current state
		matrix<N,T>& identity();
		matrix<N,T>& transpose();
		matrix<N,T>& adjugate();
		matrix<N,T>& invert();
		// Those functions don't change the current state
		matrix<N,T> getTransposed()	const;
		matrix<N,T> getAdjugate()	const;
		matrix<N,T> getInvert()		const;
		T determinant()				const;

		// Retrieve a row
		row_t row(int i) const;
		// Retrieve a col
		col_t col(int j) const;

		// Easy static access
		template<int Row, int Col>
		T& m();
		template<int Row, int Col>
		const T& m() const;

		// Easy dynamic access
		T&		 m(int at_row, int at_col);
		const T& m(int at_row, int at_col) const;

		// Unary minus : doesn't change the current state
		const matrix<N,T> operator -() const;

		// Operators that change the current matrix state
		matrix<N,T>& operator +=(const matrix<N,T> &rhs);
		matrix<N,T>& operator -=(const matrix<N,T> &rhs);
		matrix<N,T>& operator *=(const matrix<N,T> &rhs);
		matrix<N,T>& operator *=(T rhs);
		matrix<N,T>& operator /=(T rhs);
	};
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	// Operators that don't change any of the operand current state
	//----------------------------------------------------------------------------------------------
	// Addition
	template<int N, typename T>
	const matrix<N,T> operator +(const matrix<N,T> &lhs, const matrix<N,T> &rhs);

	// Subtraction
	template<int N, typename T>
	const matrix<N,T> operator -(const matrix<N,T> &lhs, const matrix<N,T> &rhs);

	// Scalar product
	template<int N, typename T>
	const matrix<N,T> operator *(const matrix<N,T> &lhs, T rhs);
	template<int N, typename T>
	const matrix<N,T> operator *(T lhs, const matrix<N,T> &rhs);

	// Scalar division
	template<int N, typename T>
	const matrix<N,T> operator /(const matrix<N,T> &lhs, T rhs);

	// Matrix product
	template<int N, typename T>
	const matrix<N,T> operator *(const matrix<N,T> &lhs, const matrix<N,T> &rhs);
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,T>& identity_matrix();

	template<int N, typename T>
	inline std::ostream& operator <<(std::ostream &out, const matrix<N,T> &rhs);
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#include "matrix.inl"

#endif /* DJAH_MATH_BASIC_MATRIX_HPP */