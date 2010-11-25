#ifndef DJAH_MATH_QUATERNION_HPP
#define DJAH_MATH_QUATERNION_HPP

#include "detail/vector_base.hpp"

namespace djah { namespace math {

	template<typename T>
	class quaternion : public detail::vector_base<4,T>
	{
	public:

		// Init from values
		explicit quaternion(T x = T(0), T y = T(0), T z = T(0), T w = T(1));
		// Init with an array
		quaternion(const T (&array)[size_]);
		// Init from a base class element
		quaternion(const detail::vector_base<size_,T> &v);

		// Quaternion members accessors
		T& x() { return data_[0]; }
		T& y() { return data_[1]; }
		T& z() { return data_[2]; }
		T& w() { return data_[3]; }

		// Const quaternion member accessors
		const T& x() const { return data_[0]; }
		const T& y() const { return data_[1]; }
		const T& z() const { return data_[2]; }
		const T& w() const { return data_[3]; }

		// Get magnitude
		T magnitude() const;
		// Normalize this vector
		quaternion<T>& normalize();
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


		// Useful constant
		static const quaternion<T> identity;
	};


	// Useful constant
	template<typename T>
	const quaternion<T> quaternion<T>::identity;


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

	// Quaternion product
	template<typename T>
	const quaternion<T> operator *(const quaternion<T> &lhs, const quaternion<T> &rhs);


	// Useful type definitions
	typedef quaternion<float>  quatf;
	typedef quaternion<double> quatd;

} /*math*/ } /*djah*/


#include "quaternion.inl"


#endif /* DJAH_MATH_QUATERNION_HPP */