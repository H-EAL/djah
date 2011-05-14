#ifndef DJAH_MATH_VECTOR_UTILS_HPP
#define DJAH_MATH_VECTOR_UTILS_HPP

#include "vector.hpp"

namespace djah { namespace math {

	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> create_vector(const vector<N,T> &from, const vector<N,T> &to)
	{
		return to - from;
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	template<typename T>
	inline vector<4,T> vec3_to_vec4(const vector<3,T> &v)
	{
		return resize<4>(v, T(1));
	}
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	template<typename T>
	inline vector<4,T> point3_to_point4(const vector<3,T> &p)
	{
		return resize<4>(p, T(0));
	}
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	template<typename T>
	inline vector<3,T> vec4_to_vec3(const vector<4,T> &v)
	{
		return resize<3>(v);
	}
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	template<typename T>
	inline vector<3,T> point4_to_point3(const vector<4,T> &p)
	{
		return resize<3>(p);
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline T distance_sq(const vector<N,T> &from, const vector<N,T> &to)
	{
		return create_vector(from, to).lengthSq();
	}
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline float distance(const vector<N,T> &from, const vector<N,T> &to)
	{
		return create_vector(from, to).length();
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> direction(const vector<N,T> &from, const vector<N,T> &to)
	{
		return create_vector(from, to).normalize();
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> project(const vector<N,T> &v, const vector<N,T> &proj_axis)
	{
		return ((v * proj_axis) / proj_axis.length()) * proj_axis;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> face_forward(const vector<N,T> &n, const vector<N,T> &v, const vector<N,T> &n_ref)
	{
		return (n_ref * v) < T(0) ? n : -n;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> reflect(const vector<N,T> &i, const vector<N,T> &n)
	{
		return i - T(2) * (i*n) * n;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline float oriented_angle(const vector<N,T> &start, const vector<N,T> &end)
	{
		vector<N,T> normal = cross(start, end).normalize();
		return 0.0f;
	}
	//--------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_VECTOR_UTILS_HPP */