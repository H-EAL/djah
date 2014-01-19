#ifndef DJAH_OPENGL_SAMPLER_HPP
#define DJAH_OPENGL_SAMPLER_HPP

#include <set>
#include <memory>
#include "djah/types.hpp"
#include "djah/opengl/resource.hpp"

namespace djah { namespace opengl {

	class texture;

	class sampler
		: public resource
	{
		DJAH_OPENGL_RESOURCE(texture);
		template<int Unit> friend class texture_unit;

	public:
		enum eBilinearMode
		{
			eBM_None      = 0,
			eBM_Near      = 1 << 0,
			eBM_Far       = 1 << 1,
			eBM_NearFar   = eBM_Near | eBM_Far
		};

		enum eMipmappingMode
		{
			eMM_None,
			eMM_Standard,
			eMM_Trilinear,
		};

	public:
		sampler();
		virtual ~sampler();

		void bindToUnit(unsigned int textureUnit) const;
		void unbindFromUnit(unsigned int textureUnit) const;
		void unbindFromAll() const;
		bool isBoundToAnyUnit() const { return !boundTextureUnits_.empty(); }
		bool isBoundToUnit(unsigned int textureUnit) const { return boundTextureUnits_.find(textureUnit) != boundTextureUnits_.end(); }

		void setWrapMode(int wrapMode);
		void setFiltering(eBilinearMode bilinearMode, eMipmappingMode mipmappingMode, float maxAnisotropy);
		void setNoFiltering();
		void setBestFiltering();

		bool hasMipmappingFiltering() const;

	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

	private:
		mutable std::set<unsigned int> boundTextureUnits_;
		eBilinearMode   bilinearMode_;
		eMipmappingMode mipmappingMode_;
		float			maxAnisotropy_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_TEXTURE_HPP */
