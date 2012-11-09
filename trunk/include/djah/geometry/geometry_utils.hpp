#ifndef DJAH_GEOMETRY_UTILS_HPP
#define DJAH_GEOMETRY_UTILS_HPP

#include "math/vector3.hpp"
#include "plane.hpp"
#include "sphere.hpp"

namespace djah { namespace geometry {

	//--------------------------------------------------------------------------
	template<typename T>
	plane<T> make_plane(const math::vector3<T> &p0, const math::vector3<T> &p1, const math::vector3<T> &p2)
	{
		const math::vector3<T> v = p1 - p0;
		const math::vector3<T> u = p2 - p0;
		const math::vector3<T> n = math::cross(v,u);

		return plane<T>(-n*p0, n);
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	template<typename T>
	T distance_from_plane(const math::vector3<T> &point, const plane<T> &plane)
	{
		const math::vector3<T> n(plane.normal().getNormalized());
		return abs(n * point + plane.distance());
	}
	//--------------------------------------------------------------------------

} /*geometry*/ } /*djah*/

#endif /* DJAH_GEOMETRY_UTILS_HPP */