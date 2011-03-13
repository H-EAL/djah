#include "resources/font.hpp"
#include "system/opengl_include.hpp"

namespace djah { namespace resources {
	/*
	unsigned int _pow2(unsigned int i)
	{
		unsigned int p2;
		for (p2=1; p2<i; p2<<=1);
		return p2;
	}


	//----------------------------------------------------------------------------------------------
	font::font(const glyph_bitmap (&glyphs)[255])
	{
		for(unsigned char c = 0; c < 255; ++c)
		{
			int w = static_cast<int>(glyphs[c].width);
			int h = static_cast<int>(glyphs[c].height);

			char_map_[c].offset = glyphs[c].offset;
			char_map_[c].position.x = static_cast<float>(w);
			char_map_[c].position.y = static_cast<float>(h);

			if( glyphs[c].bitmap.size() > 0 )
			{
				int width = _pow2(w);
				int height = _pow2(h);
				float s = static_cast<float>(w) / static_cast<float>(width);
				float t = static_cast<float>(h) / static_cast<float>(height);
				char_map_[c].tex_coord.x = s;
				char_map_[c].tex_coord.y = t;
				char_map_[c].tex = new video::ogl::texture(width, height);

				byte *expanded = new byte[2*width*height];
				for(int j = 0; j < height; ++j)
				{
					for(int i = 0; i < width; ++i)
					{
						expanded[2*(i+j*width)] = expanded[2*(i+j*width)+1] = (i>=w||j>=h)? 0 : glyphs[c].bitmap[i+w*j];
					}
				}
				char_map_[c].tex->setPixelBuffer(expanded);
				delete [] expanded;
			}
			else
				char_map_[c].tex = 0;
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	font::~font()
	{
		for(unsigned char c = 0; c < 255; ++c)
			delete char_map_[c].tex;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void font::print(const std::string &text, int x, int y)
	{
		std::string::const_iterator it;
		std::string::const_iterator it_end = text.end();
		glPushMatrix();
		glScalef(2.0f, -2.0f, 2.0f);
		for(it = text.begin(); it != it_end; ++it)
		{
			const character &c = char_map_[static_cast<unsigned char>(*it)];
			glPushMatrix();
			glTranslatef(x, -y + c.offset.y, 0.0f);
			if( c.tex && c.tex->isValid() )
			{
				c.tex->bind();
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0,0);
					glVertex2f(0.0f, c.position.y);

					glTexCoord2f(0.0f, c.tex_coord.y);
					glVertex2f(0.0f, 0.0f);

					glTexCoord2f(c.tex_coord.x, c.tex_coord.y);
					glVertex2f(c.position.x, 0.0f);

					glTexCoord2f(c.tex_coord.x, 0.0f);
					glVertex2f(c.position.x, c.position.y);
				}
				glEnd();
			}
			glPopMatrix();
			glTranslatef(c.position.x, 0.0f, 0.0f);
		}
		glPopMatrix();
	}
	//----------------------------------------------------------------------------------------------
	*/

} /*resources*/ } /*djah*/