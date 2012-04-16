#ifndef RESOURCE_FINDER_HPP
#define RESOURCE_FINDER_HPP

template<typename T>
boost::shared_ptr<T> find_resource(const std::string &url)
{
	DJAH_AUTO_PROFILE("RESOURCE MGR " + url);
	static resources::default_media_manager s_dmm;
	static bool initialized = false;
	if(!initialized)
	{
		s_dmm.registerLoader( new resources::image_loader, "png tga jpg");
		initialized = true;
	}

	// Get it from the resource manager
	boost::shared_ptr<T> res = resources::resource_manager::get().find<T>(url);
	// If not found try to load it through the media manager
	if( !res && (res = boost::shared_ptr<T>(s_dmm.loadFromUrl<T>(url))) )
		resources::resource_manager::get().add(url, res);
	else if( !res )
		DJAH_BEGIN_LOG(EWL_CRITICAL) << "[ResourceManager] Unable to find the following resource: " << url << DJAH_END_LOG();

	return res;
}

#endif /* RESOURCE_FINDER_HPP */