#ifndef RESOURCE_FINDER_HPP
#define RESOURCE_FINDER_HPP

#include <memory>

#include "djah/opengl/texture.hpp"
#include "game/resources/default_asset_finder.hpp"


namespace djah { namespace d3d {

	typedef std::shared_ptr<djah::opengl::texture> texture_sptr;

	class texture_manager
		: public utils::singleton<texture_manager>
	{
		DJAH_MAKE_SINGLETON(texture_manager);

	public:
		texture_sptr find(const std::string &textureName)
		{
			auto it = textures_.find(textureName);

			if( it == textures_.end() )
			{
                game::resources::image_sptr img = game::resources::default_asset_finder::get().load<game::resources::image>("textures/" + textureName);
				if( img )
				{
					texture_sptr pNewTex( new opengl::texture(GL_RGB, img->width(), img->height(), true) );
					GLenum pixelFormat = img->channels() == 1 ? GL_RED : GL_RGB;			
					pNewTex->setPixelBuffer(pixelFormat, GL_UNSIGNED_BYTE, img->pixels());

					it = textures_.insert( texture_map_t::value_type(textureName, pNewTex) ).first;
				}
			}

			return it->second;
		}

	private:
		texture_manager() {}
		~texture_manager() {}

	private:
		typedef std::map<std::string, texture_sptr> texture_map_t;
		texture_map_t textures_;
	};
}}

#endif /* RESOURCE_FINDER_HPP */