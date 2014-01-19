#ifndef DJAH_OPENGL_TEXTURE_UNIT_HPP
#define DJAH_OPENGL_TEXTURE_UNIT_HPP

#include "djah/opengl/texture.hpp"
#include "djah/opengl/sampler.hpp"
#include "djah/opengl/capabilities.hpp"

namespace djah { namespace opengl {

	template<int Unit = 0>
	class texture_unit
	{
	public:
		static void set_active()
		{
			if( ensure(Unit < capabilities::value_of<int>(GL_MAX_TEXTURE_IMAGE_UNITS)) )
			{
				if( !is_active() )
				{
					glActiveTexture(GL_TEXTURE0 + Unit);
					check( active_unit() == Unit );
				}
			}
		}

		static void bind(const std::shared_ptr<texture> &pTexture, const std::shared_ptr<sampler> &pSampler)
		{
			if( ensure(is_active()) )
			{
				unbind();

				pTexture->bindToUnit(Unit);
				pSampler->bindToUnit(Unit);

				pTexture_ = pTexture;
				pSampler_ = pSampler;
			}
		}

		static void unbind()
		{
			if( std::shared_ptr<texture> pPreviousTexture = pTexture_.lock() )
			{
				if( ensure(is_active()) )
				{
					pPreviousTexture->unbindFromUnit(Unit);
					pTexture_.reset();
				}
			}

			if( std::shared_ptr<sampler> pPreviousSampler = pSampler_.lock() )
			{
				pPreviousSampler->unbindFromUnit(Unit);
				pSampler_.reset();
			}
		}

		static void activate_and_bind(const std::shared_ptr<texture> &pTexture, const std::shared_ptr<sampler> &pSampler)
		{
			set_active();
			bind(pTexture, pSampler);
		}

		static bool is_active()
		{
			return active_unit() == Unit;
		}

		static std::shared_ptr<texture> bound_texture()
		{
			return pTexture_.lock();
		}

		static std::shared_ptr<sampler> bound_sampler()
		{
			return pSampler_.lock();
		}

	private:
		static int active_unit()
		{
			return capabilities::value_of<int>(GL_ACTIVE_TEXTURE, true) - GL_TEXTURE0;
		}

	public:
		struct scoped_binder
		{
			scoped_binder(unsigned int textureId)
				: previousTextureId_(0)
				, previousActiveUnit_(active_unit())
			{
				std::shared_ptr<texture> pTexture = texture_unit<Unit>::pTexture_.lock();
				if( pTexture )
				{
					previousTextureId_ = pTexture->id();
				}

				set_active();
				glBindTexture(GL_TEXTURE_2D, textureId);
			}

			~scoped_binder()
			{
				check( is_active() );
				glBindTexture(GL_TEXTURE_2D, previousTextureId_);
				if( previousActiveUnit_ != Unit )
				{
					glActiveTexture(GL_TEXTURE0 + previousActiveUnit_);
				}
			}

		private:
			unsigned int previousTextureId_;
			unsigned int previousActiveUnit_;
		};

	private:
		static std::weak_ptr<texture> pTexture_;
		static std::weak_ptr<sampler> pSampler_;
	};

	template<int Unit>
	std::weak_ptr<texture> texture_unit<Unit>::pTexture_;
	template<int Unit>
	std::weak_ptr<sampler> texture_unit<Unit>::pSampler_;

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_TEXTURE_UNIT_HPP */