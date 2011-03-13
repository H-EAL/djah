#include "resources/font_loader.hpp"

/*
#include <cassert>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "filesystem/stream.hpp"
*/


namespace djah { namespace resources {

	/*
	//----------------------------------------------------------------------------------------------
	FT_Library font_loader::s_library;
	bool font_loader::s_lib_initialized = false;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	font_loader::font_loader()
	{
		if(!s_lib_initialized)
		{
			assert( FT_Init_FreeType( &s_library ) == 0 );
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	font_loader::~font_loader()
	{
		FT_Done_FreeType(s_library);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	font* font_loader::loadFromStream(filesystem::stream &strm, const std::string &filename)
	{
		FT_Byte *buffer = new FT_Byte[strm.size()];
		strm.read(buffer, strm.size());

		FT_Face face;
		FT_New_Memory_Face(s_library, buffer, strm.size(), 0, &face);
		FT_Set_Char_Size(face, 0, 16<<6, 0, 0);
		FT_Set_Pixel_Sizes(face, 0, 16);

		FT_GlyphSlot slot = face->glyph;	

		glyph_bitmap bitmaps[255];

		for(unsigned char c = 0; c < 255; ++c)
		{
			if( FT_Load_Char(face, c, FT_LOAD_RENDER) == 0 )
			{
				const int size = slot->bitmap.width * slot->bitmap.rows;
				if( size > 0 )
				{
					bitmaps[c].offset.x = static_cast<float>(slot->advance.x >> 6);
					bitmaps[c].offset.y = static_cast<float>((slot->metrics.horiBearingY - slot->metrics.height) >> 6);
					bitmaps[c].width = static_cast<float>(slot->bitmap.width);
					bitmaps[c].height = static_cast<float>(slot->bitmap.rows);
					bitmaps[c].bitmap.resize(size);
					memcpy(&bitmaps[c].bitmap[0], slot->bitmap.buffer, size);
				}
				// Space
				else if(c == 32)
				{
					bitmaps[c].width = 4.0f;
				}
			}
		}

		FT_Done_Face(face);

		delete [] buffer;

		return new font(bitmaps);
	}
	//----------------------------------------------------------------------------------------------
	*/

} /*resources*/ } /*djah*/