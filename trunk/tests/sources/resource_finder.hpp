#ifndef RESOURCE_FINDER_HPP
#define RESOURCE_FINDER_HPP

#include <memory>
#include "djah/resources/image_loader.hpp"
#include "djah/resources/mesh_loader.hpp"
#include "djah/resources/asset_warehouse.hpp"
#include "djah/resources/asset_finder.hpp"
#include "djah/opengl/texture.hpp"
#include "djah/filesystem/browser.hpp"
#include "djah/system/gl.hpp"
#include "profiler.hpp"


//--------------------------------------------------------------------------------------------------
template<typename T>
inline std::shared_ptr<T> find_resource(const std::string &url)
{
	//DJAH_AUTO_PROFILE("Loading " + url);
	static resources::default_asset_finder s_asset_finder;
	static bool initialized = false;
	if(!initialized)
	{
		s_asset_finder.registerLoader(std::make_shared<resources::loader<resources::image>>(), "png tga jpg");
		s_asset_finder.registerLoader(std::make_shared<resources::loader<resources::mesh>>(), "mesh");
		initialized = true;
	}
	return s_asset_finder.get<T>(url);
}
//--------------------------------------------------------------------------------------------------

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
				resources::image_sptr img = assetFinder_.get<resources::image>("textures/"+ textureName);
				if( img )
				{
					texture_sptr pNewTex( new opengl::texture(GL_RGB, img->width(), img->height()) );
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
		texture_manager()
		{
			assetFinder_.registerLoader(std::make_shared<resources::loader<resources::image>>(), "png jpg tga");
		}
		virtual ~texture_manager() {}

	private:
		typedef std::map<std::string, texture_sptr> texture_map_t;
		texture_map_t textures_;
		resources::asset_finder<TYPELIST(resources::image), false> assetFinder_;
	};
}}

#endif /* RESOURCE_FINDER_HPP */