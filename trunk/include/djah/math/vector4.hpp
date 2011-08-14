#ifndef DJAH_MATH_VECTOR4_HPP
#define DJAH_MATH_VECTOR4_HPP

#include "vector.hpp"

namespace djah { namespace math {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct vector_base<4,T>
	{
		vector_base() { memset(data, 0, 4*sizeof(T)); }
		vector_base(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
		
		union
		{
			struct { T x, y, z, w; };
			T data[4];
		};

		// Usefull constants
		static const vector<4,T> x_axis;
		static const vector<4,T> y_axis;
		static const vector<4,T> z_axis;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	template<typename T>
	const vector<4,T> vector_base<4,T>::x_axis = vector<4,T>(1,0,0,0);
	template<typename T>
	const vector<4,T> vector_base<4,T>::y_axis = vector<4,T>(0,1,0,0);
	template<typename T>
	const vector<4,T> vector_base<4,T>::z_axis = vector<4,T>(0,0,1,0);
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	// Cross product
	template<typename T>
	inline const vector<4,T> cross(const vector<4,T> &lhs, const vector<4,T> &rhs)
	{
		return vector<4,T>
		(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x,
			T(0)
		);
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline const vector<4,T> operator ^(const vector<4,T> &lhs, const vector<4,T> &rhs)
	{
		return cross(lhs, rhs);
	}
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	// Useful type definitions
	typedef vector<4,unsigned char>		vector4ub;
	typedef vector<4,unsigned short>	vector4us;
	typedef vector<4,unsigned int>		vector4ui;
	typedef vector<4,char>				vector4b;
	typedef vector<4,short>				vector4s;
	typedef vector<4,int>				vector4i;
	typedef vector<4,float>				vector4f;
	typedef vector<4,double>			vector4d;

	typedef vector4ub					point4ub;
	typedef vector4us					point4us;
	typedef vector4ui					point4ui;
	typedef vector4b					point4b;
	typedef vector4s					point4s;
	typedef vector4i					point4i;
	typedef vector4f					point4f;
	typedef vector4d					point4d;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_VECTOR4_HPP */