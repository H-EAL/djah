#ifndef DJAH_MATH_VECTOR2_HPP
#define DJAH_MATH_VECTOR2_HPP

#include "vector_base.hpp"

namespace djah { namespace math {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct vector_base<2,T>
	{
		vector_base() { memset(data, 0, 2*sizeof(T)); }
		vector_base(T _x, T _y) : x(_x), y(_y) {}
		
		union
		{
			struct { T x, y; };
			T data[2];
		};
	};
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	// Useful type definitions
	typedef vector<2,unsigned char>		vector2ub;
	typedef vector<2,unsigned short>	vector2us;
	typedef vector<2,unsigned int>		vector2ui;
	typedef vector<2,char>				vector2b;
	typedef vector<2,short>				vector2s;
	typedef vector<2,int>				vector2i;
	typedef vector<2,float>				vector2f;
	typedef vector<2,double>			vector2d;

	typedef vector2ub					point2ub;
	typedef vector2us					point2us;
	typedef vector2ui					point2ui;
	typedef vector2b					point2b;
	typedef vector2s					point2s;
	typedef vector2i					point2i;
	typedef vector2f					point2f;
	typedef vector2d					point2d;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_VECTOR2_HPP */