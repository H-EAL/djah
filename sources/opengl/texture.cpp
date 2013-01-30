#include "djah/system/gl.hpp"
#include "djah/opengl/opengl_logger.hpp"
#include "djah/opengl/texture.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	u8 texture::sTextureUnit_ = 0;
	//----------------------------------------------------------------------------------------------
	u8 texture::active_unit()
	{
		return sTextureUnit_;
	}
	//----------------------------------------------------------------------------------------------
	void texture::set_active_unit(u8 unit)
	{
		if( sTextureUnit_ != unit )
		{
			sTextureUnit_ = unit;
			glActiveTexture(GL_TEXTURE0 + unit);
		}
	}
	//----------------------------------------------------------------------------------------------


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
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::bind() const
	{
		monitor<texture>::bind(id_);
		glBindTexture(GL_TEXTURE_2D, id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::unbind()
	{
		monitor<texture>::bind(0);
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
	void texture::setPixelBuffer(int pixelFormat, int pixelDataType, const byte *pixelBuffer)
	{
		monitor<texture>::assert_if_not_bound(id_);

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
		monitor<texture>::assert_if_not_bound(id_);

		DJAH_ASSERT((xOffset >= 0) && (yOffset >= 0) && (width >= 0) && (height >= 0));
		DJAH_ASSERT((xOffset + width)  < width_);
		DJAH_ASSERT((yOffset + height) < height_);

		glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, pixelFormat, pixelDataType, subPixelBuffer);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::setWrapMode(int wrapMode)
	{
		monitor<texture>::assert_if_not_bound(id_);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::setFiltering(eBilinearMode bilinearMode, eMimappingMode mipmappingMode)
	{
		monitor<texture>::assert_if_not_bound(id_);

		const int magFilter = (bilinearMode & eBM_Near) ? GL_LINEAR : GL_NEAREST;
		// Filter for objects that are close to the camera
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		const bool bilinearFar = (bilinearMode & eBM_Far) != 0;
		int minFilter = bilinearFar ? GL_LINEAR : GL_NEAREST;

		if( hasMipmapping_ )
		{
			switch( mipmappingMode )
			{
			case eMM_Standard:
				minFilter = bilinearFar ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
				break;

			case eMM_Trilinear:
				minFilter = bilinearFar ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
				break;

			default:
				DJAH_OPENGL_WARNING() << "Invalid mipmapping filtering set" << DJAH_END_LOG();
			case eMM_None:
				break;
			}
		}

		// Filter for objects that are far away from the camera
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::setNoFiltering()
	{
		setFiltering(eBM_None, eMM_None);
	}
	//----------------------------------------------------------------------------------------------
	void texture::setBestFiltering()
	{
		setFiltering(eBM_NearFar, eMM_Trilinear);
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
