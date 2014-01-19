#ifndef DJAH_MATH_QUATERNION_UTILS_HPP
#define DJAH_MATH_QUATERNION_UTILS_HPP

#include <cmath>
#include "vector_utils.hpp"
#include "quaternion.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T> make_quaternion(radian<T> angle, vector<3,T> axis)
	{
		const T theta = angle / T(2);
		if( axis != vector<3,T>::null_vector )
			axis.normalize() *= sin(theta);
		return quaternion<T>(axis.x, axis.y, axis.z, cos(theta));
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T> make_quaternion(radian<T> angle, const vector<4,T> &axis)
	{
		return make_quaternion(angle, resize<3>(axis));
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T> make_quaternion(radian<T> angle, T u_x, T u_y, T u_z)
	{
		return make_quaternion(angle, vector<3,T>(u_x, u_y, u_z));
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T> make_quaternion(const vector<3,T> &from_vec, const vector<3,T> &to_vec)
	{
		radian<T>   angle;
		vector<3,T> axis;
		std::tie(angle, axis) = oriented_angle(from_vec, to_vec);
		return make_quaternion(angle, axis);
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline vector<3,T> rotate(quaternion<T> q, const vector<3,T> &p)
	{
		const quaternion<T> qP(p.x, p.y, p.z, T(0));
		const quaternion<T> qC(q.normalize().getConjugate());
		const quaternion<T> qR(q * qP * qC);
		return vector<3,T>(qR.x, qR.y, qR.z);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline vector<4,T> rotate(quaternion<T> q, const vector<4,T> &p)
	{
		return point3_to_point4( rotate(q, resize<3>(p)) );
	}
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_QUATERNION_UTILS_HPP */