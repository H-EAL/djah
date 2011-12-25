#ifndef DJAH_MATH_MATRIX3_HPP
#define DJAH_MATH_MATRIX3_HPP

#include "matrix.hpp"

namespace djah { namespace math {
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct matrix_base<3,3,T>
	{
		matrix_base() { memset(data, 0, 3*3*sizeof(T)); }
		matrix_base(T _11_, T _12_, T _13_,  T _21_, T _22_, T _23_,  T _31_, T _32_, T _33_)
			: _11(_11_), _12(_12_), _13(_13_)
			, _21(_21_), _22(_22_), _23(_23_)
			, _31(_31_), _32(_32_), _33(_33_)
		{}
		
		union
		{
			struct
			{
				T _11, _12, _13;
				T _21, _22, _23;
				T _31, _32, _33;
			};
			T data[3*3];
		};

		// Useful constant
		static const matrix<3,3,T> mat_identity;
	};
	//----------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------
	template<typename T>
	const matrix<3,3,T> matrix_base<3,3,T>::mat_identity = matrix<3,3,T>(1,0,0, 0,1,0, 0,0,1);
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	typedef matrix<3,3,int>		matrix3i;
	typedef matrix<3,3,float>	matrix3f;
	typedef matrix<3,3,double>	matrix3d;
	//----------------------------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_MATRIX3_HPP */