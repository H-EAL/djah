#include "video/font_engine.hpp"

#include <vector>
#include <cassert>

#include "video/ogl/buffers/texture.hpp"
#include "system/opengl_include.hpp"
#include "filesystem/browser.hpp"
#include "log/logger.hpp"

namespace djah { namespace video {
	
	//----------------------------------------------------------------------------------------------
	struct font_data
	{
		struct char_metrics
		{
			math::vector2f offset;
			math::vector2f dimension;
			math::vector2f tex_coord;
			ogl::texture* tex;

		} metrics[UCHAR_MAX];
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	font_engine::font_engine()
		: current_font_(0)
		, color_(1.0f, 1.0f, 1.0f)
	{
		assert( FT_Init_FreeType( &ft_library_ ) == 0 );
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	font_engine::~font_engine()
	{
		FT_Done_FreeType(ft_library_);

		font_map_t::iterator it;
		font_map_t::iterator it_end = font_map_.end();
		for(it = font_map_.begin(); it != it_end; ++it)
			delete it->second;
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	void font_engine::setFontsPath(const std::string &fontsPath)
	{
		fonts_path_ = fontsPath;
		if( fontsPath[fontsPath.size() - 1] != '/' )
			fonts_path_ += '/';
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	void font_engine::setFont(const std::string &fontName, u8 size)
	{
		font_traits traits = { fontName, size };
		font_map_t::const_iterator it = font_map_.find( traits );
		if( it != font_map_.end() )
		{
			current_font_ = it->second;
		}
		else if( font_data *fd = newFont(traits) )
		{
			font_map_.insert( font_map_t::value_type(traits, fd) );
			current_font_ = fd;
		}
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	void font_engine::setColor(const color &c)
	{
		color_ = c;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void font_engine::print(const std::string &text, int x, int y) const
	{
		if( !current_font_ )
			return;

		std::string::const_iterator it;
		std::string::const_iterator it_end = text.end();
		
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glColor3fv(color_.data);
		for(it = text.begin(); it != it_end; ++it)
		{
			const font_data::char_metrics &c = current_font_->metrics[*it];
			glPushMatrix();
			glTranslatef((float)x, y - c.offset.y, 0.0f);
			if( c.tex && c.tex->isValid() )
			{
				c.tex->bind();
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0f, c.tex_coord.y);
					glVertex2f(0.0f, 0.0f);

					glTexCoord2f(c.tex_coord.x, c.tex_coord.y);
					glVertex2f(c.dimension.x, 0.0f);

					glTexCoord2f(c.tex_coord.x, 0.0f);
					glVertex2f(c.dimension.x, -c.dimension.y);

					glTexCoord2f(0,0);
					glVertex2f(0.0f, -c.dimension.y);
				}
				glEnd();
			}
			glPopMatrix();
			glTranslatef(c.dimension.x, 0.0f, 0.0f);
		}
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	//----------------------------------------------------------------------------------------------
	
	unsigned int _pow2(unsigned int i)
	{
		unsigned int p2;
		for (p2=1; p2<i; p2<<=1);
		return p2;
	}
	
	//----------------------------------------------------------------------------------------------
	font_data* font_engine::newFont(const font_traits &traits)
	{
		font_data *new_font = 0;
		filesystem::stream_ptr strm = filesystem::browser::get().openReadStream(fonts_path_ + traits.name);

		if( strm )
		{
			const size_t buffer_size = strm->size();
			FT_Byte *buffer = new FT_Byte[buffer_size];
			strm->read(buffer, buffer_size);

			FT_Face face;
			FT_New_Memory_Face(ft_library_, buffer, buffer_size, 0, &face);
			FT_Set_Char_Size(face, 0, traits.size << 6, 0, 0);
			FT_Set_Pixel_Sizes(face, 0, traits.size);
			
			FT_GlyphSlot slot = face->glyph;

			new_font = new font_data;

			DJAH_LOG_TODO("Save all chars in a single texture");
			for(u8 c = 0; c < UCHAR_MAX; ++c)
			{
				if( FT_Load_Char(face, c, FT_LOAD_RENDER) == 0 )
				{
					const int w = slot->bitmap.width;
					const int h = slot->bitmap.rows;
					const int size = w * h;
					
					new_font->metrics[c].offset.x = static_cast<float>(slot->advance.x >> 6);
					new_font->metrics[c].offset.y = static_cast<float>((slot->metrics.horiBearingY - slot->metrics.height) >> 6);
						
					if( size > 0 )
					{
						new_font->metrics[c].dimension.x = static_cast<float>(slot->bitmap.width);
						new_font->metrics[c].dimension.y = static_cast<float>(slot->bitmap.rows);
						
						const int width = _pow2(w);
						const int height = _pow2(h);
						new_font->metrics[c].tex_coord.x = static_cast<float>(w) / static_cast<float>(width);
						new_font->metrics[c].tex_coord.y = static_cast<float>(h) / static_cast<float>(height);

						std::vector<u8> bitmap;
						bitmap.resize(size);
						memcpy(&bitmap[0], slot->bitmap.buffer, size);

						new_font->metrics[c].tex = new video::ogl::texture(width, height);

						byte *expanded = new byte[2*width*height];
						for(int j = 0; j < height; ++j)
						{
							for(int i = 0; i < width; ++i)
							{
								expanded[2*(i+j*width)] = expanded[2*(i+j*width)+1] = (i>=w||j>=h) ? 0 : bitmap[i+w*j];
							}
						}
						new_font->metrics[c].tex->setPixelBuffer(expanded, true);
						delete [] expanded;
					}
					else
					{
						new_font->metrics[c].tex = 0;
						new_font->metrics[c].dimension.x = new_font->metrics[c].offset.x;
					}
				}
			}
			
			FT_Done_Face(face);

			delete [] buffer;
		}

		return new_font;
	}
	//----------------------------------------------------------------------------------------------

} /*video*/ } /*djah*/