#ifndef DJAH_MATH_QUATERNION_HPP
#define DJAH_MATH_QUATERNION_HPP

#include <cassert>
#include "vector3.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	class quaternion
	{
	public:

		// Default: construct with identity
		quaternion();
		// Init from values
		quaternion(T _x, T _y, T _z, T _w);
		// Init with an array
		quaternion(const T (&array)[4]);

		// Get magnitude
		T magnitude() const;
		// Normalize this quaternion
		quaternion<T>& normalize();
		// Retrieve a normalized copy of this quaternion
		quaternion<T> getNormalized() const;
		// Replace this quaternion by its conjugate
		quaternion<T>& conjugate();
		// Retrieve conjugate
		quaternion<T>  getConjugate() const;

		// Negate
		const quaternion<T> operator -() const;

		// Addition
		quaternion<T>& operator +=(const quaternion<T> &rhs);
		// Subtraction
		quaternion<T>& operator -=(const quaternion<T> &rhs);
		// Scalar product
		quaternion<T>& operator *=(T rhs);
		// Scalar division
		quaternion<T>& operator /=(T rhs);
		// Quaternion product
		quaternion<T>& operator *=(const quaternion<T> &rhs);

		// Accessors
		T&		 operator [](unsigned int i);
		const T& operator [](unsigned int i) const;

		
		// Actual data
		union
		{
			struct { T x, y, z, w; };
			T data[4];
		};

		// Useful constant
		static const quaternion<T> identity;
	};
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	// Addition
	template<typename T>
	quaternion<T> operator +(const quaternion<T> &lhs, const quaternion<T> &rhs);
	// Subtraction
	template<typename T>
	quaternion<T> operator -(const quaternion<T> &lhs, const quaternion<T> &rhs);
	// Scalar product
	template<typename T>
	quaternion<T> operator *(const quaternion<T> &lhs, T rhs);
	template<typename T>
	quaternion<T> operator *(T lhs, const quaternion<T> &rhs);
	// Scalar division
	template<typename T>
	quaternion<T> operator /(const quaternion<T> &lhs, T rhs);
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	// Quaternion product
	template<typename T>
	quaternion<T> operator *(const quaternion<T> &lhs, const quaternion<T> &rhs);
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	// (In/Out)put (from/to) stream
	//----------------------------------------------------------------------------------------------
	template<typename T>
	std::ostream& operator <<(std::ostream &out, const quaternion<T> &rhs);
	//----------------------------------------------------------------------------------------------
	template<typename T>
	std::istream& operator >>(std::istream &in, quaternion<T> &rhs);
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	// Useful type definitions
	typedef quaternion<float>  quatf;
	typedef quaternion<double> quatd;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#include "quaternion.inl"

#endif /* DJAH_MATH_QUATERNION_HPP */