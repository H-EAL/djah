#ifndef DJAH_GEOMETRY_TRIANGLE_HPP
#define DJAH_GEOMETRY_TRIANGLE_HPP

#include "math/vector.hpp"

namespace djah { namespace geometry {

	template<int N, typename T>
	class triangle
	{
	public:

		// Init with 3 points
		triangle(const math::vector<N,T> &_a, const math::vector<N,T> &_b, const math::vector<N,T> &_c)
			: a(_a), b(_b), c(_c) {}

		math::vector<N,T> a;
		math::vector<N,T> b;
		math::vector<N,T> c;
	};


	// Useful type definitions
	typedef triangle<2,float>  triangle2f;
	typedef triangle<2,double> triangle2d;

	typedef triangle<3,float>  triangle3f;
	typedef triangle<3,double> triangle3d;

} /*geometry*/ } /*djah*/

#endif /* DJAH_GEOMETRY_TRIANGLE_HPP */