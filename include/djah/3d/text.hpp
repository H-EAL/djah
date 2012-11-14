#ifndef DJAH_VIDEO_TEXT_HPP
#define DJAH_VIDEO_TEXT_HPP

#include <string>
#include "../types.hpp"
#include "../math/vector2.hpp"
#include "../math/vector3.hpp"

namespace djah { namespace d3d {

	class text
		: public std::string
	{
	public:
		struct properties
		{
			properties(const std::string &name = "arial.ttf", u8 size = 14, const math::vector3f &color = math::vector3f(1.0f, 1.0f, 1.0f))
				: font_name(name)
				, font_size(size)
				, font_color(color)
			{}

			std::string font_name;
			u8 font_size;
			math::vector3f font_color;
		};

		explicit text(const std::string &str = "", int x = 0, int y = 0, const properties &props = properties());
		text& operator =(const std::string &str) { std::string::operator=(str); return (*this); }

		void setPosition(const math::vector2i &position) { position_ = position; }
		void setProperties(const properties &props) { properties_ = props; }

		void draw() const;

	private:
		properties properties_;
		math::vector2i position_;
	};

} /*d3d*/ } /*djah*/

#endif /* DJAH_VIDEO_TEXT_HPP */