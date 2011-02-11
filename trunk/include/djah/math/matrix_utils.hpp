#ifndef DJAH_MATH_MATRIX_UTILS_HPP
#define DJAH_MATH_MATRIX_UTILS_HPP

#include "matrix2.hpp"
#include "matrix3.hpp"
#include "matrix4.hpp"

#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

namespace djah { namespace math {

	//------------------------------------------------------------------------------
	template<typename T>
	inline basic_matrix<4,T> make_translation(T t_x, T t_y, T t_z, T t_w = T(1))
	{
		const T m[4][4] =
		{
			{1,0,0,t_x},
			{0,1,0,t_y},
			{0,0,1,t_z},
			{0,0,0,t_w}
		};
		return basic_matrix<4,T>(m);
	}
	//------------------------------------------------------------------------------
	template<typename T>
	inline basic_matrix<4,T> make_translation(const vector<3,T> &v)
	{
		return make_translation(v.x, v.y, v.z);
	}
	//------------------------------------------------------------------------------
	template<typename T>
	inline basic_matrix<4,T> make_translation(const vector<4,T> &v)
	{
		return make_translation(v.x, v.y, v.z, v.w);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline basic_matrix<4,T> make_scale(T t_x, T t_y, T t_z, T t_w = T(1))
	{
		const T m[4][4] =
		{
			{t_x,   0,   0,   0},
			{  0, t_y,   0,   0},
			{  0,   0, t_z,   0},
			{  0,   0,   0, t_w}
		};
		return basic_matrix<4,T>(m);
	}
	//------------------------------------------------------------------------------
	template<typename T>
	inline basic_matrix<4,T> make_scale(const vector<3,T> &v)
	{
		return make_scale(v.x(), v.y(), v.z());
	}
	//------------------------------------------------------------------------------
	template<typename T>
	inline basic_matrix<4,T> make_scale(const vector<4,T> &v)
	{
		return make_scale(v.x, v.y, v.z, v.w);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	template<typename T>
	inline basic_matrix<4,T> make_rotation(T angle, vector<3,T> axis)
	{
		axis.normalize();

		const T u_x  = axis.x; 
		const T u_y  = axis.y; 
		const T u_z  = axis.z;
		const T u_x2 = u_x * u_x;
		const T u_y2 = u_y * u_y;
		const T u_z2 = u_z * u_z;
		const T c    = cos(angle);
		const T s    = sin(angle);

		basic_matrix<4,T> R;

		R.m<1,1>() = u_x2 + (1 - u_x2) * c;
		R.m<1,2>() = u_x * u_y * (1 - c) - u_z * s;
		R.m<1,3>() = u_x * u_z * (1 - c) + u_y * s;

		R.m<2,1>() = u_x * u_y * (1 - c) + u_z * s;
		R.m<2,2>() = u_y2 + (1 - u_y2) * c;
		R.m<2,3>() = u_y * u_z * (1 - c) - u_x * s;

		R.m<3,1>() = u_x * u_z * (1 - c) - u_y * s;
		R.m<3,2>() = u_y * u_z * (1 - c) + u_x * s;
		R.m<3,3>() = u_z2 + (1 - u_z2) * c;

		R.m<4,4>() = 1;

		return R;
	}
	//------------------------------------------------------------------------------
	template<typename A, typename T>
	inline basic_matrix<4,T> make_rotation(A angle, const vector<4,T> &axis)
	{
		return make_rotation(angle, vector<3,T>(axis.x(), axis.y(), axis.z()));
	}
	//------------------------------------------------------------------------------
	template<typename A, typename T>
	inline basic_matrix<4,T> make_rotation(A angle, T t_x, T t_y, T t_z)
	{
		return make_rotation(angle, vector<3,T>(t_x, t_y, t_z));
	}
	//------------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> operator *(const basic_matrix<N,T> &m, const vector<N,T> &v)
	{
		typedef vector<N,T> vec_t;
		vec_t result;
		for(size_t i = 0; i < N; ++i)
		{
			const vec_t r_i( m.row(i) );
			result[i] = r_i * v;
		}

		return result;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline vector<SIZE,T> transform(const basic_matrix<SIZE,T> &m, const vector<SIZE,T> &v)
	{
		return m*v;
	}
	//--------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_MATRIX_UTILS_HPP */