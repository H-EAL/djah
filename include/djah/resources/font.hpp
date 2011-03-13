#ifndef DJAH_RESOURCES_FONT_HPP
#define DJAH_RESOURCES_FONT_HPP

/*
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <vector>
#include "resource_base.hpp"
#include "../video/ogl/buffers/texture.hpp"
#include "../math/vector2.hpp"
*/

namespace djah { namespace resources {
/*

	struct glyph_bitmap
	{
		float width;
		float height;
		math::vector2f offset;
		std::vector<char> bitmap;
	};

	class font
		: public resource_base
	{
	public:
		font(const glyph_bitmap (&glyps)[255]);
		~font();

		void print(const std::string &text, int x = 0, int y = 16);

		struct character
		{
			math::vector2f offset;
			math::vector2f position;
			math::vector2f tex_coord;
			video::ogl::texture *tex;
		};

		character char_map_[255];
	};

	FT_Face face_;

	typedef boost::shared_ptr<font> font_ptr;
	*/
} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_FONT_HPP */