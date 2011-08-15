#ifndef DJAH_MATH_MATRIX_UTILS_HPP
#define DJAH_MATH_MATRIX_UTILS_HPP

#include "matrix2.hpp"
#include "matrix3.hpp"
#include "matrix4.hpp"

#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

namespace djah { namespace math {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline matrix<4,T> make_translation(T t_x, T t_y, T t_z)
	{
		const T m[4][4] =
		{
			{1,0,0,t_x},
			{0,1,0,t_y},
			{0,0,1,t_z},
			{0,0,0,  1}
		};
		return matrix<4,T>(m);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline matrix<4,T> make_translation(const vector<3,T> &v)
	{
		return make_translation(v.x, v.y, v.z);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline matrix<4,T> make_translation(const vector<4,T> &v)
	{
		return make_translation(v.x, v.y, v.z);
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline matrix<4,T> make_scale(T t_x, T t_y, T t_z)
	{
		const T m[4][4] =
		{
			{t_x,   0,   0,   0},
			{  0, t_y,   0,   0},
			{  0,   0, t_z,   0},
			{  0,   0,   0,   1}
		};
		return matrix<4,T>(m);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline matrix<4,T> make_scale(const vector<3,T> &v)
	{
		return make_scale(v.x, v.y, v.z);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline matrix<4,T> make_scale(const vector<4,T> &v)
	{
		return make_scale(v.x, v.y, v.z);
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline matrix<4,T> make_rotation(T angle, vector<3,T> axis)
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

		T m[4][4] =
		{
			{ u_x2 + (1 - u_x2) * c,			u_x * u_y * (1 - c) + u_z * s,		u_x * u_z * (1 - c) - u_y * s,	0	},
			{ u_x * u_y * (1 - c) - u_z * s,	u_y2 + (1 - u_y2) * c,				u_y * u_z * (1 - c) + u_x * s,	0	},
			{ u_x * u_z * (1 - c) + u_y * s,	u_y * u_z * (1 - c) - u_x * s,		u_z2 + (1 - u_z2) * c,			0	},
			{ 0,								0,									0,								1	}
		};

		return matrix<4,T>(m);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline matrix<4,T> make_rotation(T angle, const vector<4,T> &axis)
	{
		return make_rotation(angle, resize<3>(axis));
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline matrix<4,T> make_rotation(T angle, T t_x, T t_y, T t_z)
	{
		return make_rotation(angle, vector<3,T>(t_x, t_y, t_z));
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> transform(const matrix<N,T> &m, const vector<N,T> &v)
	{
		vector<N,T> result;

		for(size_t r = 0; r < N; ++r)
		{
			const vector<N,T> row( m.row(r) );
			result.data[r] = row * v;
		}

		return result;
	}
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> operator *(const matrix<N,T> &m, const vector<N,T> &v)
	{
		return transform(m,v);
	}
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_MATRIX_UTILS_HPP */