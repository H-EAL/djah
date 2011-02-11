#ifndef DJAH_GEOMETRY_SPHERE_HPP
#define DJAH_GEOMETRY_SPHERE_HPP

#include "math/vector3.hpp"

namespace djah { namespace geometry {

	template<typename T>
	class sphere
	{
	public:

		// A sphere is just a 3D point and a radius
		sphere(T radius = T(1), const math::vector<3,T> &center = math::vector3<T>())
			: radius_(radius), center_(center) {}

		// Read/Write accessors
		T&                       radius()       { return radius_; }
		math::vector<3,T>&       center()       { return center_; }
		// Read-only accessors
		const T&                 radius() const { return radius_; }
		const math::vector<3,T>& center() const { return center_; }

		// Indicates if a point is inside the sphere
		bool isInside(const math::vector<3,T> &point) const
		{
			const T distance = math::distance(point, center_);
			return distance <= radius_;
		}


	private:

		T                 radius_;
		math::vector<3,T> center_;
	};


	// Comparison operators
	template<typename T>
	inline bool operator  <(const sphere<T> &lhs, const sphere<T> &rhs)
	{ return lhs.radius() < rhs.radius(); }

	template<typename T>
	inline bool operator <=(const sphere<T> &lhs, const sphere<T> &rhs)
	{ return (lhs < rhs) && !(rhs < lhs); }

	template<typename T>
	inline bool operator  >(const sphere<T> &lhs, const sphere<T> &rhs)
	{ return !(lhs < rhs) && (rhs < lhs); }

	template<typename T>
	inline bool operator >=(const sphere<T> &lhs, const sphere<T> &rhs)
	{ return !(lhs < rhs); }

	template<typename T>
	inline bool operator ==(const sphere<T> &lhs, const sphere<T> &rhs)
	{ return !(lhs < rhs) && !(rhs < lhs); }

	template<typename T>
	inline bool operator !=(const sphere<T> &lhs, const sphere<T> &rhs)
	{ return !(lhs == rhs); }


	// Useful type definitions
	typedef sphere<float>  sphere_f;
	typedef sphere<double> sphere_d;

} /*geometry*/ } /*djah*/

#endif /* DJAH_GEOMETRY_SPHERE_HPP */