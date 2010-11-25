#ifndef DJAH_GEOMETRY_RECT_HPP
#define DJAH_GEOMETRY_RECT_HPP

#include "../math/vector2.hpp"

namespace djah { namespace geometry {

	template<typename T>
	class rect
	{
	public:

		rect(T left = T(0), T top = T(0), T right = T(0), T bottom = T(0));
		rect(const math::vector2<T> &topLeft, const math::vector2<T> &bottomRight);
		rect(const math::vector2<T> &topLeftPosition, T width, T height);

		T width() const;
		T height() const;

		// Read/Write accessors
		math::vector2<T>& topLeft();
		math::vector2<T>& bottomRight();

		// Read-only accessors
		const math::vector2<T>& topLeft() const;
		const math::vector2<T>& bottomRight() const;

	private:

		math::vector2<T> topLeft_;
		math::vector2<T> bottomRight_;
	};


	// Useful type definitions
	typedef rect<short>		rect_s;
	typedef rect<int>		rect_i;
	typedef rect<float>		rect_f;
	typedef rect<double>	rect_d;

} /*geometry*/ } /*djah*/


#include "rect.inl"


#endif /* DJAH_GEOMETRY_RECT_HPP */