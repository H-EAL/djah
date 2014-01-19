#ifndef DJAH_RESOURCES_ASSET_FINDER_HPP
#define DJAH_RESOURCES_ASSET_FINDER_HPP

#include <map>
#include <string>
#include <memory>
#include <type_traits>

#include "djah/core/hierarchy_generation.hpp"
#include "djah/core/string_utils.hpp"

#include "djah/filesystem/browser.hpp"

#include "djah/resources/asset_warehouse.hpp"
#include "djah/resources/loaders.hpp"


namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct loader_holder
	{
		typedef std::shared_ptr<loader<T>> loader_sptr;
		typedef std::map<std::string, loader_sptr> loaders_map_t;
		loaders_map_t loaders_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	typedef TYPELIST(image, mesh, data_object<>) DefaultAssetsTypes;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ExtraAssetsTypes_ = utils::nulltype, bool UseDefaultTypes_ = true>
	class asset_finder
		: public utils::gen_scatter_hierarchy
				 <
					typename std::conditional
					<
						UseDefaultTypes_,
						typename utils::tl::append<DefaultAssetsTypes,ExtraAssetsTypes_>::Result,
						ExtraAssetsTypes_
					>::type,
					loader_holder
				  >
	{
	public:
		typedef typename typename std::conditional
		<
			UseDefaultTypes_,
			typename utils::tl::append<DefaultAssetsTypes,ExtraAssetsTypes_>::Result,
			ExtraAssetsTypes_
		>::type AssetsTypeList;

	public:
		template<typename T>
		void registerLoader(const std::string &extensions);

		template<typename T>
		std::shared_ptr<T> get(const std::string &url, bool loadIfNotFound = true);

	private:
		template<typename T>
		std::shared_ptr<T> loadFromUrl(const std::string &url);

		template<typename T>
		void saveToUrl(const T &obj, const std::string &url);

		template<typename T>
		typename loader_holder<T>::loader_sptr findLoader(const std::string &url);
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	typedef asset_finder<> default_asset_finder;
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/

#include "asset_finder.inl"

#endif /* DJAH_RESOURCES_ASSET_FINDER_HPP */