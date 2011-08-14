#ifndef DJAH_GEOMETRY_RAY_HPP
#define DJAH_GEOMETRY_RAY_HPP

#include "math/vector3.hpp"

namespace djah { namespace geometry {

	template<int N, typename T>
	class ray
	{
	public:

		// A ray has an origin point and points to a direction
		ray(const math::vector<N,T> &origin, const math::vector<N,T> &direction)
			: origin_(origin), direction_(direction) {}

		// Read/Write accessors
		math::vector<N,T>& origin()    { return origin_;    }
		math::vector<N,T>& direction() { return direction_; }
		// Read-only accessors
		const math::vector<N,T>& origin()    const { return origin_;    }
		const math::vector<N,T>& direction() const { return direction_; }

	private:

		math::vector<N,T> origin_;
		math::vector<N,T> direction_;
	};


	// Useful type definitions
	typedef ray<2,float>  ray2f;
	typedef ray<2,double> ray2d;

	typedef ray<3,float>  ray3f;
	typedef ray<3,double> ray3d;

} /*geometry*/ } /*djah*/

#endif /* DJAH_GEOMETRY_RAY_HPP */