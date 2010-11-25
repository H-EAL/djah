#ifndef DJAH_MATH_QUATERNION_UTILS_HPP
#define DJAH_MATH_QUATERNION_UTILS_HPP

#include "quaternion.hpp"
#include <cmath>

namespace djah { namespace math {

	//--------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T> make_quaternion(T angle, vector3<T> axis)
	{
		const T theta = angle / T(2);
		axis.normalize() *= sin(theta);
		return quaternion<T>(axis.x(), axis.y(), axis.z(), cos(theta));
	}
	//--------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T> make_quaternion(T angle, const vector4<T> &axis)
	{
		return make_quaternion(angle, vec4_to_vec3(axis));
	}
	//--------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T> make_quaternion(T angle, T u_x, T u_y, T u_z)
	{
		return make_quaternion(angle, vector3<T>(u_x, u_y, u_z));
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<typename T>
	inline vector3<T> rotate(quaternion<T> q, const vector3<T> &p)
	{
		const quaternion<T> qP(p.x(), p.y(), p.z(), T(0));
		const quaternion<T> qC(q.normalize().getConjugate());
		const quaternion<T> qR(q * qP * qC);
		return vector3<T>(qR.x(), qR.y(), qR.z());
	}
	//--------------------------------------------------------------------------
	template<typename T>
	inline vector4<T> rotate(quaternion<T> q, const vector4<T> &p)
	{
		return point3_to_point4( rotate(q, point4_to_point3(p)) );
	}
	//--------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_QUATERNION_UTILS_HPP */