#ifndef DJAH_OPENGL_SAMPLER_HPP
#define DJAH_OPENGL_SAMPLER_HPP

#include <memory>
#include "djah/types.hpp"
#include "djah/opengl/resource.hpp"

namespace djah { namespace opengl {

	class texture;

	class sampler
		: public resource
	{
		DJAH_OPENGL_RESOURCE(texture);

	public:
		enum eBilinearMode
		{
			eBM_None      = 0,
			eBM_Near      = 1 << 0,
			eBM_Far       = 1 << 1,
			eBM_NearFar   = eBM_Near | eBM_Far
		};

		enum eMimappingMode
		{
			eMM_None,
			eMM_Standard,
			eMM_Trilinear,
		};

	public:
		sampler(std::shared_ptr<texture> pTexture);
		virtual ~sampler();

		void bind(int textureUnit = -1) const;
		static void unbind(int textureUnit = -1);

		void setWrapMode(int wrapMode);
		void setFiltering(eBilinearMode bilinearMode, eMimappingMode mipmappingMode);
		void setNoFiltering();
		void setBestFiltering();

	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

	private:
		std::shared_ptr<texture> pTexture_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_TEXTURE_HPP */
