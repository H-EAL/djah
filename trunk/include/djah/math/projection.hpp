#ifndef DJAH_MATH_PROJECTION_HPP
#define DJAH_MATH_PROJECTION_HPP

#include "math_utils.hpp"
#include "matrix4.hpp"
#include "vector3.hpp"

namespace djah { namespace math { 

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline const matrix<4,4,T> make_perspective_projection(T fovy, T aspect, T near, T far)
	{
		const T F = T(1) / tan( math::deg_to_rad(fovy/T(2)) );
		const T n_m_f = near - far;
		const T n_p_f = near + far;
		const T n_t_f = near * far;

		return matrix<4,4,T>
		(
			F/aspect,	0,            0,                   0,
			       0,	F,            0,                   0,
			       0,	0,	n_p_f/n_m_f,	T(2)*n_t_f/n_m_f,
			       0,	0,		  T(-1),                   0
		);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline const matrix<4,4,T> make_orthographic_projection(T left, T right, T bottom, T top, T near, T far)
	{
		const T r_p_l = right + left;
		const T r_m_l = right - left;
		const T t_p_b = top + bottom;
		const T t_m_b = top - bottom;
		const T f_p_n = far + near;
		const T f_m_n = far - near;
		
		return matrix<4,4,T>
		(
			T(2)/r_m_l,          0,           0, -(r_p_l/r_m_l),
			         0, T(2)/t_m_b,           0, -(t_p_b/t_m_b),
			         0,          0, T(-2)/f_m_n, -(f_p_n/f_m_n),
			         0,          0,           0,              1
		);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline const matrix<4,4,T> make_look_at(const vector<3,T> &eye, const vector<3,T> &center, vector<3,T> up)
	{
		up.normalize();
		math::vector<3,T> forward = math::direction(eye, center);
		math::vector<3,T> side = forward.cross(up).getNormalized();
		up = side.cross(forward).getNormalized();
		
		return matrix<4,4,T>
		(
			    side.x,     side.y,     side.z,    -(side * eye),
			      up.x,       up.y,       up.z,      -(up * eye),
			-forward.x, -forward.y, -forward.z,  (forward * eye),
			         0,          0,          0,                1
		);
	}
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_PROJECTION_HPP */