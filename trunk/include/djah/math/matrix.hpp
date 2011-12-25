#ifndef DJAH_MATH_BASIC_MATRIX_HPP
#define DJAH_MATH_BASIC_MATRIX_HPP

#include <algorithm>
#include <iostream>
#include <cstring>
#include "vector.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	struct matrix_base
	{
		matrix_base() { memset(data, 0, M*N*sizeof(T)); }
		T data[M*N];
	};
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<int M, int N, typename T>
	class matrix
		: public matrix_base<M,N,T>
	{
	public:
		
		static const int nb_row = M;
		static const int nb_col = N;
		typedef T value_type;

		// A row/col type
		typedef vector<N,T> row_t;
		typedef vector<M,T> col_t;

		// Default constructor -> null matrix
		matrix();
		
		// Constructors from values, only the needed one is actually created
		// 2x2
		matrix(T _11, T _12,  T _21, T _22)
			: matrix_base<M,N,T>(_11,_12,  _21,_22) {}
		// 3x3
		matrix(T _11, T _12, T _13,  T _21, T _22, T _23,  T _31, T _32, T _33)
			: matrix_base<M,N,T>(_11,_12,_13,  _21,_22,_23,  _31,_32,_33) {}
		// 4x4
		matrix(T _11, T _12, T _13, T _14,  T _21, T _22, T _23, T _24,  T _31, T _32, T _33, T _34,  T _41, T _42, T _43, T _44)
			: matrix_base<M,N,T>(_11,_12,_13,_14,  _21,_22,_23,_24,  _31,_32,_33,_34,  _41,_42,_43,_44) {}
		// 2x3 and 3x2
		matrix(T _11, T _12, T _13,  T _21, T _22, T _23)
			: matrix_base<M,N,T>(_11,_12,_13, _21,_22,_23) {}
		// 2x4 and 4x2
		matrix(T _11, T _12, T _13, T _14,  T _21, T _22, T _23, T _24)
			: matrix_base<M,N,T>(_11,_12,_13,_14, _21,_22,_23,_24) {}
		// 3x4 and 4x3
		matrix(T _11, T _12, T _13, T _14,  T _21, T _22, T _23, T _24,  T _31, T _32, T _33, T _34)
			: matrix_base<M,N,T>(_11,_12,_13,_14,  _21,_22,_23,_24,  _31,_32,_33,_34) {}
		
		// Constructors from array
		matrix(const T (&array)[M*N]);
		matrix(const T (&array)[M][N]);
		
		// Matrix math
		// Those functions change the current state
		matrix<M,N,T>& toIdentity();
		matrix<M,N,T>& transpose();
		matrix<M,N,T>& adjugate();
		matrix<M,N,T>& invert();
		// Those functions don't change the current state
		matrix<M,N,T> getTransposed()	const;
		matrix<M,N,T> getAdjugate()		const;
		matrix<M,N,T> getInvert()		const;
		T			  determinant()		const;

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
		const matrix<M,N,T> operator -() const;

		// Operators that change the current matrix state
		matrix<M,N,T>& operator +=(const matrix<M,N,T> &rhs);
		matrix<M,N,T>& operator -=(const matrix<M,N,T> &rhs);
		matrix<M,N,T>& operator *=(const matrix<M,N,T> &rhs);
		matrix<M,N,T>& operator *=(T rhs);
		matrix<M,N,T>& operator /=(T rhs);
	};
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	// Operators that don't change any of the operand current state
	//----------------------------------------------------------------------------------------------
	// Addition
	template<int M, int N, typename T>
	const matrix<M,N,T> operator +(const matrix<M,N,T> &lhs, const matrix<M,N,T> &rhs);

	// Subtraction
	template<int M, int N, typename T>
	const matrix<M,N,T> operator -(const matrix<M,N,T> &lhs, const matrix<M,N,T> &rhs);

	// Scalar product
	template<int M, int N, typename T>
	const matrix<M,N,T> operator *(const matrix<M,N,T> &lhs, T rhs);
	template<int M, int N, typename T>
	const matrix<M,N,T> operator *(T lhs, const matrix<M,N,T> &rhs);

	// Scalar division
	template<int M, int N, typename T>
	const matrix<M,N,T> operator /(const matrix<M,N,T> &lhs, T rhs);

	// Matrix product
	template<int M, int N, int P, typename T>
	const matrix<M,P,T> operator *(const matrix<M,N,T> &lhs, const matrix<P,M,T> &rhs);
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline const matrix<N,N,T>& identity_matrix();

	template<int M, int N, typename T>
	inline std::ostream& operator <<(std::ostream &out, const matrix<M,N,T> &rhs);
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#include "matrix.inl"

#endif /* DJAH_MATH_BASIC_MATRIX_HPP */