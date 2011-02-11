#ifndef DJAH_GEOMETRY_RAY_HPP
#define DJAH_GEOMETRY_RAY_HPP

#include "math/vector3.hpp"

namespace djah { namespace geometry {

	template<typename T>
	class ray
	{
	public:

		// A ray has an origin point and points to a direction
		ray(const math::vector<3,T> &origin, const math::vector<3,T> &direction)
			: origin_(origin), direction_(direction) {}

		// Read/Write accessors
		T&                       origin()    { return origin_;    }
		math::vector<3,T>&       direction() { return direction_; }
		// Read-only accessors
		const T&                 origin()    const { return origin_;    }
		const math::vector<3,T>& direction() const { return direction_; }


	private:

		math::vector<3,T> origin_;
		math::vector<3,T> direction_;
	};


	// Useful type definitions
	typedef ray<float>  ray_f;
	typedef ray<double> ray_d;

} /*geometry*/ } /*djah*/

#endif /* DJAH_GEOMETRY_RAY_HPP */