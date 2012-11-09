#ifndef DJAH_GEOMETRY_RECT_HPP
#define DJAH_GEOMETRY_RECT_HPP

#include <limits>
#include <algorithm>
#include "../math/vector2.hpp"
#include "../math/vector_utils.hpp"

namespace djah { namespace geometry {

	template<typename T>
	class rect
	{
	public:
		rect(T left = T(0), T top = T(0), T right = T(0), T bottom = T(0));
		rect(const math::vector<2,T> &top_left, const math::vector<2,T> &bottom_right);
		rect(const math::vector<2,T> &top_left, T _width, T _height);

		void repair();
		void invalidate();
		bool isValid() const;

		T width() const;
		T height() const;
		math::vector<2,T> extent() const;

		// Read/Write accessors
		math::vector<2,T>& topLeft();
		math::vector<2,T>& bottomRight();

		// Read-only accessors
		const math::vector<2,T>& topLeft() const;
		const math::vector<2,T>& bottomRight() const;

	private:
		math::vector<2,T> topLeft_;
		math::vector<2,T> bottomRight_;
	};


	// Useful type definitions
	typedef rect<short>		rect_s;
	typedef rect<int>		rect_i;
	typedef rect<float>		rect_f;
	typedef rect<double>	rect_d;

} /*geometry*/ } /*djah*/


#include "rect.inl"


#endif /* DJAH_GEOMETRY_RECT_HPP */