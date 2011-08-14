#ifndef DJAH_GEOMETRY_PLANE_HPP
#define DJAH_GEOMETRY_PLANE_HPP

#include "math/vector3.hpp"

namespace djah { namespace geometry {

	template<typename T>
	class plane
	{
	public:

		// Init with a normal and a distance from the origin
		plane(const math::vector3<T> &normal, T distance);
		// Init with the plane equation ax + by + cz + d = 0
		plane(T a, T b, T c, T d);

		// Read/Write accessors
		T&							distance();
		math::vector<3,T>&			normal();
		T&							a();
		T&							b();
		T&							c();
		T&							d();
		// Read-only accessors
		const T&					distance() const;
		const math::vector<3,T>&	normal()   const;
		const T&					a()        const;
		const T&					b()        const;
		const T&					c()        const;
		const T&					d()        const;

		// Solve plane equation
		T solve(const math::vector<3,T> &point) const;

		// Relative positions
		enum { BEHIND_PLANE = -1, ON_PLANE, IN_FRONT_OF_PLANE };

		// Indicates if the point is (behind/on/in front of) the plane
		int relativePosition(const math::vector<3,T> &point) const;

		// Indicates if a point is on the plane
		bool isOn(const math::vector<3,T> &point)        const;
		// Indicates if a point is behind the plane
		bool isBehind(const math::vector<3,T> &point)    const;
		// Indicates if a point is in front of the plane
		bool isInFrontOf(const math::vector<3,T> &point) const;


	private:

		math::vector<3,T>	normal_;
		T					distance_;
	};


	// Useful type definitions
	typedef plane<float>  plane_f;
	typedef plane<double> plane_d;

} /*geometry*/ } /*djah*/

#include "plane.inl"

#endif /* DJAH_GEOMETRY_PLANE_HPP */