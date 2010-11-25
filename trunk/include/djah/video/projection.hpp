#ifndef DJAH_VIDEO_PROJECTION_HPP
#define DJAH_VIDEO_PROJECTION_HPP

#include "../math/matrix4.hpp"
#include "../math.hpp"

namespace djah { namespace video { 

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline math::basic_matrix<4,T> make_perspective_projection(T fovy, T aspect, T near, T far)
	{
		const T F = T(1) / tan( math::deg_to_rad(fovy/T(2)) );
		const T n_m_f = near - far;
		const T n_p_f = near + far;
		const T n_t_f = near * far;

		const T m[4][4] =
		{
			{F/aspect, 0,           0,                0},
			{       0, F,           0,                0},
			{       0, 0, n_p_f/n_m_f, T(2)*n_t_f/n_m_f},
			{       0, 0,       T(-1),                 0}
		};

		return math::basic_matrix<4,T>(m);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline math::basic_matrix<4,T> make_orthographic_projection(T near, T far, T left, T right, T top, T bottom)
	{
		const T r_p_l = right + left;
		const T r_m_l = right - left;
		const T t_p_b = top + bottom;
		const T t_m_b = top - bottom;
		const T f_p_n = far + near;
		const T f_m_n = far - near;

		const T m[4][4] =
		{
			{T(2)/r_m_l,          0,          0, r_p_l/r_m_l},
			{         0, T(2)/t_m_b,          0, t_p_b/t_m_b},
			{         0,          0, T(2)/f_m_n, f_p_n/f_m_n},
			{         0,          0,          0,           1}
		};

		return math::basic_matrix<4,T>(m);
	}
	//----------------------------------------------------------------------------------------------

} /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_PROJECTION_HPP */