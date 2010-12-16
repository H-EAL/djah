#ifndef DJAH_MATH_BASIC_VECTOR_HPP
#define DJAH_MATH_BASIC_VECTOR_HPP

#include <cassert>
#include "detail/vector_base.hpp"

#include <math.h>

namespace djah { namespace math {

	using namespace detail;

	template<size_t SIZE, typename T>
	class basic_vector
		: public vector_base<SIZE,T>
	{
	public:

		// All data zeroed
		basic_vector();
		// Init with an array
		basic_vector(const T (&array)[SIZE]);
		// Init with a vector_base
		basic_vector(const detail::vector_base<SIZE,T> &v);

		// Squared length
		T     lengthSq() const;
		// Vector magnitude in float
		float length()   const;
		// Normalize this vector
		basic_vector<SIZE,T>& normalize();
		// Return the normalized vector
		basic_vector<SIZE,T> getNormalized() const;

		// Negate
		const basic_vector<SIZE,T> operator -() const;

		// Addition
		basic_vector<SIZE,T>& operator +=(const basic_vector<SIZE,T> &rhs);
		// Subtraction
		basic_vector<SIZE,T>& operator -=(const basic_vector<SIZE,T> &rhs);
		// Scalar product
		basic_vector<SIZE,T>& operator *=(T rhs);
		// Scalar division
		basic_vector<SIZE,T>& operator /=(T rhs);


		// Useful constant
		static const basic_vector<SIZE,T> null_vec;
	};


	// Useful constant
	template<size_t SIZE, typename T>
	const basic_vector<SIZE,T> basic_vector<SIZE,T>::null_vec;


	// Addition
	template<size_t SIZE, typename T>
	const basic_vector<SIZE,T> operator +(const basic_vector<SIZE,T> &lhs, const basic_vector<SIZE,T> &rhs);
	// Subtraction
	template<size_t SIZE, typename T>
	const basic_vector<SIZE,T> operator -(const basic_vector<SIZE,T> &lhs, const basic_vector<SIZE,T> &rhs);
	// Scalar product
	template<size_t SIZE, typename T>
	const basic_vector<SIZE,T> operator *(const basic_vector<SIZE,T> &lhs, T rhs);
	template<size_t SIZE, typename T>
	const basic_vector<SIZE,T> operator *(T lhs, const basic_vector<SIZE,T> &rhs);
	// Scalar division
	template<size_t SIZE, typename T>
	const basic_vector<SIZE,T> operator /(const basic_vector<SIZE,T> &lhs, T rhs);

	// Dot product
	template<size_t SIZE, typename T>
	const T operator *(const basic_vector<SIZE,T> &lhs, const basic_vector<SIZE,T> &rhs);


} /*math*/ } /*djah*/


#include "basic_vector.inl"


#endif /* DJAH_MATH_BASIC_VECTOR_HPP */