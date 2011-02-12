#ifndef DJAH_MATH_QUATERNION_HPP
#define DJAH_MATH_QUATERNION_HPP

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	class quaternion
	{
	public:

		// Init from values
		explicit quaternion(T _x = T(0), T _y = T(0), T _z = T(0), T _w = T(1));
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
	const quaternion<T> operator +(const quaternion<T> &lhs, const quaternion<T> &rhs);
	// Subtraction
	template<typename T>
	const quaternion<T> operator -(const quaternion<T> &lhs, const quaternion<T> &rhs);
	// Scalar product
	template<typename T>
	const quaternion<T> operator *(const quaternion<T> &lhs, T rhs);
	template<typename T>
	const quaternion<T> operator *(T lhs, const quaternion<T> &rhs);
	// Scalar division
	template<typename T>
	const quaternion<T> operator /(const quaternion<T> &lhs, T rhs);
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	// Quaternion product
	template<typename T>
	const quaternion<T> operator *(const quaternion<T> &lhs, const quaternion<T> &rhs);
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	// Useful type definitions
	typedef quaternion<float>  quatf;
	typedef quaternion<double> quatd;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#include "quaternion.inl"

#endif /* DJAH_MATH_QUATERNION_HPP */