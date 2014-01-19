#ifndef DJAH_MATH_VECTOR_UTILS_HPP
#define DJAH_MATH_VECTOR_UTILS_HPP

#include <tuple>
#include "vector.hpp"
#include "angles.hpp"

namespace djah { namespace math {

	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> create_vector(const vector<N,T> &from_point, const vector<N,T> &to_point)
	{
		return to_point - from_point;
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
	inline T distance_sq(const vector<N,T> &from_point, const vector<N,T> &to_point)
	{
		return create_vector(from_point, to_point).lengthSq();
	}
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline typename vector<N,T>::float_t distance(const vector<N,T> &from_point, const vector<N,T> &to_point)
	{
		return create_vector(from_point, to_point).length();
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> direction(const vector<N,T> &from_point, const vector<N,T> &to_point)
	{
		return create_vector(from_point, to_point).normalize();
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<int N, typename T>
	inline vector<N,T> normal(const vector<N,T> &u, const vector<N,T> &v)
	{
		return u.cross(v).normalize();
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
	inline std::tuple< radian<T>, vector<N,T> >
		oriented_angle(const vector<N,T> &from_vec, const vector<N,T> &to_vec)
	{
		std::tuple< radian<T>, vector<N,T> > angle_axis;

		if( from_vec != to_vec )
		{
			const vector<N,T> &norm = normal(from_vec, to_vec);
			const vector<N,T> &orientation = to_vec.getNormalized();
			const float dotResult = orientation * norm;

			radian<T> &angle  = std::get<0>(angle_axis);
			vector<N,T> &axis = std::get<1>(angle_axis);
			if(dotResult == -1.0f)
			{
				// If dot result is -1 then from and to vectors form a 180 degree angle
				angle.value = pi;
				DJAH_LOG_TODO("Determine which axis");
			}
			else
			{
				axis  = from_vec.cross(orientation);
				angle.value = atan2( norm * axis, orientation * from_vec );
			}
		}

		return angle_axis;
	}
	//--------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_VECTOR_UTILS_HPP */