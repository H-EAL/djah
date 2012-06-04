#ifndef DJAH_MATH_MATRIX3X2_HPP
#define DJAH_MATH_MATRIX3X2_HPP

#include "matrix.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct matrix_base<3,2,T>
	{
		matrix_base() { memset(data, 0, 3*2*sizeof(T)); }
		matrix_base(T _11_, T _12_,  T _21_, T _22_,  T _31_, T _32_)
			: _11(_11_), _12(_12_)
			, _21(_21_), _22(_22_)
			, _31(_31_), _32(_32_)
		{}
		
		union
		{
			struct
			{
				T _11, _12;
				T _21, _22;
				T _31, _32;
			};
			T data[3][2];
		};
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	typedef matrix<3,2,int>		matrix3x2i;
	typedef matrix<3,2,float>	matrix3x2f;
	typedef matrix<3,2,double>	matrix3x2d;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_MATRIX3X2_HPP */