#include <algorithm>
#include "djah/system/gl.hpp"
#include "djah/opengl/opengl_logger.hpp"
#include "djah/opengl/sampler.hpp"
#include "djah/opengl/capabilities.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	sampler::sampler()
		: bilinearMode_(eBM_None)
		, mipmappingMode_(eMM_None)
		, maxAnisotropy_(1.0f)
	{
		aquire();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	sampler::~sampler()
	{
		check( !isBoundToAnyUnit() );
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::bindToUnit(unsigned int textureUnit) const
	{
		if( !isBoundToUnit(textureUnit) )
		{
			glBindSampler(textureUnit, id_);
			boundTextureUnits_.insert(textureUnit);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::unbindFromUnit(unsigned int textureUnit) const
	{
		if( ensure(isBoundToUnit(textureUnit)) )
		{
			glBindSampler(textureUnit, 0);
			boundTextureUnits_.erase(textureUnit);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::unbindFromAll() const
	{
        for (auto textureUnit : boundTextureUnits_)
        {
            unbindFromUnit(textureUnit);
        }
        
		check( boundTextureUnits_.empty() );
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
		glSamplerParameteri(id_, GL_TEXTURE_WRAP_R, wrapMode);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::setFiltering(eBilinearMode bilinearMode, eMipmappingMode mipmappingMode, float maxAnisotropy)
	{
		const int magFilter = (bilinearMode & eBM_Near) ? GL_LINEAR : GL_NEAREST;
		// Filter for objects that are close to the camera
		glSamplerParameteri(id_, GL_TEXTURE_MAG_FILTER, magFilter);

		const bool bilinearFar = (bilinearMode & eBM_Far) != 0;
		int minFilter = bilinearFar ? GL_LINEAR : GL_NEAREST;

		switch( mipmappingMode )
		{
		case eMM_None:
			break;

		case eMM_Standard:
			minFilter = bilinearFar ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
			break;

		case eMM_Trilinear:
			minFilter = bilinearFar ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR;
			break;

		default:
			DJAH_OPENGL_WARNING() << "Invalid mipmapping filtering set (" << mipmappingMode << ")" << DJAH_END_LOG();
			break;
		}

		// Filter for objects that are far away from the camera
		glSamplerParameteri(id_, GL_TEXTURE_MIN_FILTER, minFilter);

		// Set anisotropy
		glSamplerParameterf(id_, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

		// Save values
		bilinearMode_   = bilinearMode;
		mipmappingMode_ = mipmappingMode;
		maxAnisotropy_  = maxAnisotropy;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void sampler::setNoFiltering()
	{
		setFiltering(eBM_None, eMM_None, 1.0f);
	}
	//----------------------------------------------------------------------------------------------
	void sampler::setBestFiltering()
	{
		setFiltering(eBM_NearFar, eMM_Trilinear, capabilities::value_of<float>(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT));
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool sampler::hasMipmappingFiltering() const
	{
		return mipmappingMode_ != eMM_None;
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
