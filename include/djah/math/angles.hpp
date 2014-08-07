#ifndef DJAH_MATH_ANGLES_HPP
#define DJAH_MATH_ANGLES_HPP

#include "djah/math/math_utils.hpp"

namespace djah { namespace math {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline T deg_to_rad(const T degrees)
	{
		static const T coeff = T(pi) / T(180);
		return degrees * coeff;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline T rad_to_deg(const T radians)
	{
		static const T coeff = T(180) / T(pi);
		return radians * coeff;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class radian;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class degree
	{
	public:
		explicit degree(float val = 0.0f) : value(val) {}
		degree(const radian &rad);

		static degree from_radian(float val);

		radian toRadian() const;
		operator float() { return value; }

		float value;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class radian
	{
	public:
		explicit radian(float val = 0.0f) : value(val) {}
		radian(const degree &deg) : value(deg_to_rad(deg.value)) {}

		static radian from_degree(float val) { return radian(degree(val)); }

		degree toDegree() const { return degree(*this); }
		operator float() { return value; }

		float value;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	inline degree::degree(const radian &rad) : value(rad_to_deg(rad.value)) {}
	inline degree degree::from_radian(float val) { return degree(radian(val)); }
	inline radian degree::toRadian() const { return radian(*this); }
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_ANGLES_HPP */