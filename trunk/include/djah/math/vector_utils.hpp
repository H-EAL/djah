#ifndef DJAH_MATH_VECTOR_UTILS_HPP
#define DJAH_MATH_VECTOR_UTILS_HPP

#include "basic_vector.hpp"

namespace djah { namespace math {

	//--------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T> create_vector(const basic_vector<SIZE,T> &from, const basic_vector<SIZE,T> &to)
	{
		return to - from;
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	template<typename T>
	inline vector4<T> vec3_to_vec4(const vector3<T> &v)
	{
		return vector4<T>(v.x(), v.y(), v.z(), T(1));
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<typename T>
	inline vector4<T> point3_to_point4(const vector3<T> &p)
	{
		return vector4<T>(p.x(), p.y(), p.z(), T(0));
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<typename T>
	inline vector3<T> vec4_to_vec3(const vector4<T> &v)
	{
		return vector3<T>(v.x(), v.y(), v.z());
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<typename T>
	inline vector4<T> point4_to_point3(const vector3<T> &p)
	{
		return vec4_to_vec3(p);
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline T distance_sq(const basic_vector<SIZE,T> &from, const basic_vector<SIZE,T> &to)
	{
		return create_vector(from, to).lengthSq();
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline float distance(const basic_vector<SIZE,T> &from, const basic_vector<SIZE,T> &to)
	{
		return create_vector(from, to).length();
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T> direction(const basic_vector<SIZE,T> &from, const basic_vector<SIZE,T> &to)
	{
		return create_vector(from, to).normalize();
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T> project(const basic_vector<SIZE,T> &v, const basic_vector<SIZE,T> &proj_axis)
	{
		return ((v * proj_axis) / proj_axis.length()) * proj_axis;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T> face_forward(const basic_vector<SIZE,T> &n, const basic_vector<SIZE,T> &v, const basic_vector<SIZE,T> &n_ref)
	{
		return (n_ref * v) < T(0) ? n : -n;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<size_t SIZE, typename T>
	inline basic_vector<SIZE,T> reflect(const basic_vector<SIZE,T> &i, const basic_vector<SIZE,T> &n)
	{
		return i - T(2) * (i*n) * n;
	}
	//--------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_VECTOR_UTILS_HPP */