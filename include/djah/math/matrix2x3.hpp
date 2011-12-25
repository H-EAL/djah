#ifndef DJAH_MATH_MATRIX2X3_HPP
#define DJAH_MATH_MATRIX2X3_HPP

#include "matrix.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct matrix_base<2,3,T>
	{
		matrix_base() { memset(data, 0, 2*3*sizeof(T)); }
		matrix_base(T _11_, T _12_, T _13_,  T _21_, T _22_, T _23_)
			: _11(_11_), _12(_12_), _13(_13_)
			, _21(_21_), _22(_22_), _23(_23_)
		{}
		
		union
		{
			struct
			{
				T _11, _12, _13;
				T _21, _22, _23;
			};
			T data[2*3];
		};
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	typedef matrix<2,3,int>		matrix2x3i;
	typedef matrix<2,3,float>	matrix2x3f;
	typedef matrix<2,3,double>	matrix2x3d;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_MATRIX2X3_HPP */