#ifndef DJAH_MATH_VECTOR3_HPP
#define DJAH_MATH_VECTOR3_HPP

#include "vector.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct vector_base<3,T>
	{
		vector_base() { memset(data, 0, 3*sizeof(T)); }
		vector_base(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
		
		union
		{
			struct { T x, y, z; };
			T data[3];
		};

		// Usefull constants
		static const vector<3,T> x_axis;
		static const vector<3,T> y_axis;
		static const vector<3,T> z_axis;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	const vector<3,T> vector_base<3,T>::x_axis = vector<3,T>(1,0,0);
	template<typename T>
	const vector<3,T> vector_base<3,T>::y_axis = vector<3,T>(0,1,0);
	template<typename T>
	const vector<3,T> vector_base<3,T>::z_axis = vector<3,T>(0,0,1);
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	// Cross product
	template<typename T>
	inline vector<3,T> cross(const vector<3,T> &lhs, const vector<3,T> &rhs)
	{
		return vector<3,T>
		(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	const vector<3,T> operator ^(const vector<3,T> &lhs, const vector<3,T> &rhs)
	{
		return cross(lhs, rhs);
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	// Useful type definitions
	typedef vector<3,unsigned char>		vector3ub;
	typedef vector<3,unsigned short>	vector3us;
	typedef vector<3,unsigned int>		vector3ui;
	typedef vector<3,char>				vector3b;
	typedef vector<3,short>				vector3s;
	typedef vector<3,int>				vector3i;
	typedef vector<3,float>				vector3f;
	typedef vector<3,double>			vector3d;

	typedef vector3ub					point3ub;
	typedef vector3us					point3us;
	typedef vector3ui					point3ui;
	typedef vector3b					point3b;
	typedef vector3s					point3s;
	typedef vector3i					point3i;
	typedef vector3f					point3f;
	typedef vector3d					point3d;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_VECTOR3_HPP */