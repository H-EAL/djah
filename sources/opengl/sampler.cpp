#include "djah/system/gl.hpp"
#include "djah/opengl/opengl_logger.hpp"
#include "djah/opengl/sampler.hpp"
#include "djah/opengl/texture.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	sampler::sampler(std::shared_ptr<texture> pTexture)
		: pTexture_(pTexture)
	{
		aquire();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	sampler::~sampler()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::bind(int textureUnit) const
	{
		if(textureUnit == -1)
		{
			textureUnit = texture::active_unit();
		}
		glBindSampler(textureUnit, id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::unbind(int textureUnit)
	{
		if(textureUnit == -1)
		{
			textureUnit = texture::active_unit();
		}
		glBindSampler(0, 0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::aquire()
	{
		glGenSamplers(1, &id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::release()
	{
		glDeleteSamplers(1, &id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool sampler::isValidResource() const
	{
		return glIsSampler(id_) != 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::setWrapMode(int wrapMode)
	{
		glSamplerParameteri(id_, GL_TEXTURE_WRAP_S, wrapMode);
		glSamplerParameteri(id_, GL_TEXTURE_WRAP_T, wrapMode);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::setFiltering(eBilinearMode bilinearMode, eMimappingMode mipmappingMode)
	{
		check(pTexture_);

		const int magFilter = (bilinearMode & eBM_Near) ? GL_LINEAR : GL_NEAREST;
		// Filter for objects that are close to the camera
		glSamplerParameteri(id_, GL_TEXTURE_MAG_FILTER, magFilter);

		const bool bilinearFar = (bilinearMode & eBM_Far) != 0;
		int minFilter = bilinearFar ? GL_LINEAR : GL_NEAREST;

		if( pTexture_->hasMipmapping() )
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
		glSamplerParameteri(id_, GL_TEXTURE_MIN_FILTER, minFilter);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::setNoFiltering()
	{
		setFiltering(eBM_None, eMM_None);
	}
	//----------------------------------------------------------------------------------------------
	void sampler::setBestFiltering()
	{
		setFiltering(eBM_NearFar, eMM_Trilinear);
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
