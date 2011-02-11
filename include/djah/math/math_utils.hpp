#ifndef DJAH_MATH_UTILS_HPP
#define DJAH_MATH_UTILS_HPP

#include "splines.hpp"
#include "matrix_utils.hpp"
#include "vector_utils.hpp"
#include "quaternion_utils.hpp"

namespace djah { namespace math {

	//--------------------------------------------------------------------------
	static const double pi_d = 3.1415926535897932384626433;
	static const float  pi_f = static_cast<float>(pi_d);
	static const float  pi   = pi_f;

	static const float pi_times_2 = pi * 2.0f;

	static const float pi_over_2 = pi / 2.0f;
	static const float pi_over_3 = pi / 3.0f;
	static const float pi_over_4 = pi / 4.0f;
	static const float pi_over_6 = pi / 6.0f;
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	template<typename T>
	inline T deg_to_rad(const T degrees)
	{
		static const T coeff = T(pi) / T(180);
		return degrees * coeff;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<typename T>
	inline T rad_to_deg(const T radians)
	{
		static const T coeff = T(180) / T(pi);
		return radians * coeff;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<typename T>
	inline basic_matrix<4,T> quat_to_matrix4(const quaternion<T> &q)
	{
		const T qx = q.x();
		const T qy = q.y();
		const T qz = q.z();
		const T qw = q.w();
		const T qx2 = qx * qx;
		const T qy2 = qy * qy;
		const T qz2 = qz * qz;
		
		const T m[4][4] =
		{
			{1 - 2*qy2 - 2*qz2, 2*qx*qy - 2*qz*qw, 2*qx*qz + 2*qy*qw, 0},
			{2*qx*qy + 2*qz*qw, 1 - 2*qx2 - 2*qz2, 2*qy*qz - 2*qx*qw, 0},
			{2*qx*qz - 2*qy*qw, 2*qy*qz + 2*qx*qw, 1 - 2*qx2 - 2*qy2, 0},
			{                0,                 0,                 0, 1}
		};
		return basic_matrix<4,T>(m);
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	template<typename T>
	inline quaternion<T> matrix4_to_quat(const basic_matrix<4,T> &mat)
	{
		const T w = sqrt(T(1) + mat.m<1,1>() + mat.m<2,2>() + mat.m<3,3>()) / T(2);
		const T four_w = T(4) * w;
		const T x = (mat.m<3,2>() - mat.m<2,3>()) / four_w;
		const T y = (mat.m<1,3>() - mat.m<3,1>()) / four_w;
		const T z = (mat.m<2,1>() - mat.m<1,2>()) / four_w;
		return quaternion<T>(x,y,z,w);
	}
	//--------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_UTILS_HPP */