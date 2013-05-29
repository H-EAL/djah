#ifndef RESOURCE_FINDER_HPP
#define RESOURCE_FINDER_HPP

#include <memory>
#include "djah/resources/image_loader.hpp"
#include "djah/resources/mesh_loader.hpp"
#include "djah/resources/resource_manager.hpp"
#include "djah/resources/media_manager.hpp"
#include "djah/opengl/texture.hpp"
#include "djah/filesystem/browser.hpp"
#include "djah/system/gl.hpp"
#include "profiler.hpp"


//--------------------------------------------------------------------------------------------------
template<typename T>
inline std::shared_ptr<T> find_resource(const std::string &url)
{
	//DJAH_AUTO_PROFILE("Loading " + url);
	static resources::default_media_manager s_dmm;
	static bool initialized = false;
	if(!initialized)
	{
		s_dmm.registerLoader( new resources::image_loader, "png tga jpg");
		s_dmm.registerLoader( new resources::mesh_loader, "mesh");
		initialized = true;
	}

	// Get it from the resource manager
	std::shared_ptr<T> res = resources::resource_manager::get().find<T>(url);
	// If not found try to load it through the media manager
	if( !res && (res = std::shared_ptr<T>(s_dmm.loadFromUrl<T>(url))) )
	{
		resources::resource_manager::get().add(url, res);
	}
	else if( !res )
	{
		DJAH_GLOBAL_CRITICAL()
			<< "[ResourceManager] Unable to find the following resource: "
			<< url
			<< DJAH_END_LOG();
	}

	return res;
}
//--------------------------------------------------------------------------------------------------

namespace djah { namespace d3d {

	typedef std::shared_ptr<djah::opengl::texture> texture_ptr;
	class texture_manager
		: public utils::singleton<texture_manager>
	{
		DJAH_MAKE_SINGLETON(texture_manager);

	public:
		texture_ptr find(const std::string &textureName)
		{
			auto it = textures_.find(textureName);

			if( it == textures_.end() )
			{
				resources::image_ptr img = find_resource<resources::image>("textures/"+ textureName);
				if( img )
				{
					texture_ptr pNewTex( new opengl::texture(GL_RGB, img->width(), img->height()) );
					pNewTex->bind();
					pNewTex->setBestFiltering();
					GLenum pixelFormat = img->channels() == 1 ? GL_RED : GL_BGR;			
					pNewTex->setPixelBuffer(pixelFormat, GL_UNSIGNED_BYTE, img->pixels());
					pNewTex->unbind();

					it = textures_.insert( texture_map_t::value_type(textureName, pNewTex) ).first;
				}
			}

			return it->second;
		}

	private:
		texture_manager() {}
		virtual ~texture_manager() {}

	private:
		typedef std::map<std::string, texture_ptr> texture_map_t;
		texture_map_t textures_;
	};
}}

#endif /* RESOURCE_FINDER_HPP */