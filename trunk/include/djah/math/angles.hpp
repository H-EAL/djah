#ifndef DJAH_MATH_ANGLES_HPP
#define DJAH_MATH_ANGLES_HPP

#include "math_utils.hpp"

namespace djah { namespace math {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	class radian;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	class degree
	{
	public:
		explicit degree(T val = T(0)) : value(val) {}
		template<typename U>
		degree(radian<U> rad) : value(math::rad_to_deg(static_cast<T>(rad.value))) {}

		static degree<T> from_radian(T val) { return degree<T>(radian<T>(val)); }

		radian<T> toRadian() const { return radian<T>(*this); }
		operator radian<T>() { return toRadian(); }
		operator T() { return value; }

		T value;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	class radian
	{
	public:
		explicit radian(T val = T(0)) : value(val) {}
		template<typename U>
		radian(degree<U> deg) : value(math::deg_to_rad(static_cast<T>(deg.value))) {}

		static radian<T> from_degree(T val) { return radian<T>(degree<T>(val)); }

		degree<T> toDegree() const { return degree<T>(*this); }
		operator degree<T>() { return degree<T>(*this); }
		operator T() { return value; }

		T value;
	};
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_ANGLES_HPP */