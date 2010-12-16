#ifndef DJAH_RESOURCES_MEDIA_MANAGER_HPP
#define DJAH_RESOURCES_MEDIA_MANAGER_HPP

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "../utils/hierarchy_generation.hpp"
#include "../utils/string_utils.hpp"
#include "../fs/filesystem.hpp"
#include "loader.hpp"
#include "image.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct media_holder
	{
		typedef boost::shared_ptr< loader<T> > loader_t_ptr;
		typedef std::map<std::string, loader_t_ptr> loaders_map_t;
		loaders_map_t loaders_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	typedef TYPELIST_1(image) DefaultTypes;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraTypes_ = utils::nulltype, typename DefaultTypes_ = DefaultTypes>
	class media_manager
		: public utils::gen_scatter_hierarchy<typename utils::tl::append<DefaultTypes_,ExtraTypes_>::Result, media_holder>
	{
	public:

		template<typename T>
		void registerLoader(loader<T> *loader, const std::string &extensions);

		template<typename T>
		T* loadFromUrl(const std::string &url);

		template<typename T>
		void saveToUrl(const T &obj, const std::string &url);

	private:

		template<typename T>
		typename media_holder<T>::loader_t_ptr findLoader(const std::string &url);
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	typedef media_manager<> default_media_manager;
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/

#include "media_manager.inl"

#endif /* DJAH_RESOURCES_MEDIA_MANAGER_HPP */