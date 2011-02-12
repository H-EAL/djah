#ifndef DJAH_MATH_MATRIX2_HPP
#define DJAH_MATH_MATRIX2_HPP

#include "matrix.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct matrix_base<2,T>
	{
		matrix_base() { memset(data, 0, 2*2*sizeof(T)); }
		matrix_base(T _11_, T _12_,  T _21_, T _22_)
			: _11(_11_), _12(_12_)
			, _21(_21_), _22(_22_)
		{}
		
		union
		{
			struct
			{
				T _11, _21;
				T _12, _22;
			};
			T data[2*2];
		};
	};
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	typedef matrix<2,int>		matrix2i;
	typedef matrix<2,float>		matrix2f;
	typedef matrix<2,double>	matrix2d;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_MATRIX2_HPP */