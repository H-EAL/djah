#ifndef RESOURCE_FINDER_HPP
#define RESOURCE_FINDER_HPP

#include <memory>
#include "djah/resources/image_loader.hpp"
#include "djah/resources/resource_manager.hpp"
#include "djah/filesystem/browser.hpp"
#include "profiler.hpp"

//--------------------------------------------------------------------------------------------------
template<typename T>
inline std::shared_ptr<T> find_resource(const std::string &url)
{
	DJAH_AUTO_PROFILE("Loading " + url);
	static resources::default_media_manager s_dmm;
	static bool initialized = false;
	if(!initialized)
	{
		s_dmm.registerLoader( new resources::image_loader, "png tga jpg");
		initialized = true;
	}

	// Get it from the resource manager
	std::shared_ptr<T> res = resources::resource_manager::get().find<T>(url);
	// If not found try to load it through the media manager
	if( !res && (res = std::shared_ptr<T>(s_dmm.loadFromUrl<T>(url))) )
		resources::resource_manager::get().add(url, res);
	else if( !res )
		DJAH_BEGIN_LOG(critical) << "[ResourceManager] Unable to find the following resource: " << url << DJAH_END_LOG();

	return res;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
inline std::string loadShaderSource(const std::string &url)
{
	std::string source;

	djah::filesystem::stream_ptr strm = djah::filesystem::browser::get().openReadStream(url);
	if( strm )
	{
		unsigned int src_size = strm->size();
		source.resize(src_size);
		strm->read(&source[0], src_size);
	}

	return source;
}
//--------------------------------------------------------------------------------------------------

#endif /* RESOURCE_FINDER_HPP */