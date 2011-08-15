#ifndef DJAH_MATH_SPLINES_HPP
#define DJAH_MATH_SPLINES_HPP

#include "vector_utils.hpp"

namespace djah { namespace math {

	//--------------------------------------------------------------------------
	template<typename T, typename V>
	const V catmull_rom(T t, const V &p0, const V &p1, const V &p2, const V &p3)
	{
		const T t2 = t*t;
		const T t3 = t*t*t;
		return T(0.5) * (
					        (T(2)*p1) +
							(-p0 + p2) * t +
							(T(2)*p0 - T(5)*p1 + T(4)*p2 - p3) * t2 +
							(-p0 + T(3)*p1 - T(3)*p2 + p3) * t3
						);
	}
	//--------------------------------------------------------------------------

} /*math*/ } /*djah*/

#endif /* DJAH_MATH_SPLINES_HPP */