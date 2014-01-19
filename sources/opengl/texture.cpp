#include "djah/system/gl.hpp"
#include "djah/opengl/opengl_logger.hpp"
#include "djah/opengl/texture.hpp"
#include "djah/opengl/texture_unit.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	texture::texture(int internalFormat, int width, int height, bool generateMipmap)
		: internalFormat_(internalFormat)
		, width_(width)
		, height_(height)
		, hasMipmapping_(generateMipmap)
	{
		aquire();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	texture::~texture()
	{
		check( !isBoundToAnyUnit() );
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::bindToUnit(unsigned int textureUnit) const
	{
		if( !isBoundToUnit(textureUnit) )
		{
			glBindTexture(GL_TEXTURE_2D, id_);
			boundTextureUnits_.insert(textureUnit);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::unbindFromUnit(unsigned int textureUnit) const
	{
		if( ensure(isBoundToUnit(textureUnit)) )
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			boundTextureUnits_.erase(textureUnit);
		}
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
	void texture::setPixelBuffer(int pixelFormat, int pixelDataType, const byte *pixelBuffer)
	{
		texture_unit<0>::scoped_binder __binder(id_);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat_, width_, height_, 0, pixelFormat, pixelDataType, pixelBuffer);

		if( hasMipmapping_ )
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::updatePixelBuffer(int xOffset, int yOffset, int width, int height, int pixelFormat, int pixelDataType, const byte *subPixelBuffer)
	{
		texture_unit<0>::scoped_binder __binder(id_);

		check((xOffset >= 0) && (yOffset >= 0) && (width >= 0) && (height >= 0));
		check((xOffset + width)  < width_);
		check((yOffset + height) < height_);

		glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, pixelFormat, pixelDataType, subPixelBuffer);
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
