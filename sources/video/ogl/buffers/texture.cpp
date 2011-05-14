#include "system/opengl_include.hpp"
#include "video/ogl/buffers/texture.hpp"

namespace djah { namespace video { namespace ogl {

	//----------------------------------------------------------------------------------------------
	texture::texture(int width, int height)
		: width_(width)
		, height_(height)
	{
		aquire();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	texture::~texture()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::bind(int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::aquire()
	{
		glGenTextures(1, &id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::release()
	{
		glDeleteTextures(1, &id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool texture::isValidResource() const
	{
		return glIsTexture(id_) != 0;
	}
	//----------------------------------------------------------------------------------------------
	

	//----------------------------------------------------------------------------------------------
	void texture::setPixelBuffer(const byte *pixelBuffer, bool luminance)
	{
		bind();

		if( luminance )
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16, width_, height_, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, pixelBuffer);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_BGR, GL_UNSIGNED_BYTE, pixelBuffer);	
		}
	}
	//----------------------------------------------------------------------------------------------


} /*ogl*/ } /*video*/ } /*djah*/
