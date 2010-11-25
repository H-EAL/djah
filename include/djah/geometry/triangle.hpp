#ifndef DJAH_GEOMETRY_TRIANGLE_HPP
#define DJAH_GEOMETRY_TRIANGLE_HPP

#include "math/vector3.hpp"

namespace djah { namespace geometry {

	template<typename T>
	class triangle
	{
	public:

		// Init with 3 points
		triangle(const math::vector3<T> &a, const math::vector3<T> &b, const math::vector3<T> &c);

		// Read/Write accessors
		math::vector3<T>&		a();
		math::vector3<T>&		b();
		math::vector3<T>&		c();
		// Read-only accessors
		const math::vector3<T>&	a() const;
		const math::vector3<T>&	b() const;
		const math::vector3<T>&	c() const;


	private:

		math::vector3<T> a_;
		math::vector3<T> b_;
		math::vector3<T> c_;
	};


	// Useful type definitions
	typedef triangle<float>  triangle_f;
	typedef triangle<double> triangle_d;

} /*geometry*/ } /*djah*/


#include "triangle.inl"


#endif /* DJAH_GEOMETRY_TRIANGLE_HPP */