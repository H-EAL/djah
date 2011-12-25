#ifndef DJAH_MATH_MATRIX4X2_HPP
#define DJAH_MATH_MATRIX4X2_HPP

#include "matrix.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct matrix_base<4,2,T>
	{
		matrix_base() { memset(data, 0, 4*2*sizeof(T)); }
		matrix_base(T _11_, T _12_,  T _21_, T _22_,  T _31_, T _32_,  T _41_, T _42_)
			: _11(_11_), _12(_12_)
			, _21(_21_), _22(_22_)
			, _31(_31_), _32(_32_)
			, _41(_41_), _42(_42_)
		{}
		
		union
		{
			struct
			{
				T _11, _12;
				T _21, _22;
				T _31, _32;
				T _41, _42;
			};
			T data[4*2];
		};
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	typedef matrix<4,2,int>		matrix4x2i;
	typedef matrix<4,2,float>	matrix4x2f;
	typedef matrix<4,2,double>	matrix4x2d;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_MATRIX4X2_HPP */