#ifndef DJAH_MATH_MATRIX2X4_HPP
#define DJAH_MATH_MATRIX2X4_HPP

#include "matrix.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct matrix_base<2,4,T>
	{
		matrix_base() { memset(data, 0, 2*4*sizeof(T)); }
		matrix_base(T _11_, T _12_, T _13_, T _14_,  T _21_, T _22_, T _23_, T _24_)
			: _11(_11_), _12(_12_), _13(_13_), _14(_14_)
			, _21(_21_), _22(_22_), _23(_23_), _24(_24_)
		{}
		
		union
		{
			struct
			{
				T _11, _12, _13, _14;
				T _21, _22, _23, _24;
			};
			T data[2][4];
		};
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	typedef matrix<2,4,int>		matrix2x4i;
	typedef matrix<2,4,float>	matrix2x4f;
	typedef matrix<2,4,double>	matrix2x4d;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_MATRIX2X4_HPP */