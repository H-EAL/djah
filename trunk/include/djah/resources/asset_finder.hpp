#ifndef DJAH_RESOURCES_ASSET_FINDER_HPP
#define DJAH_RESOURCES_ASSET_FINDER_HPP

#include <set>
#include <string>
#include <memory>
#include <type_traits>

#include "djah/core/hierarchy_generation.hpp"
#include "djah/core/string_utils.hpp"
#include "djah/core/singleton.hpp"

#include "djah/filesystem/browser.hpp"

#include "djah/resources/asset_warehouse.hpp"
#include "djah/resources/loaders.hpp"

#include "djah/debug/log.hpp"


namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	template<typename AssetType>
	struct asset_extensions
	{
		typedef std::set<std::string> extensions_map_t;
		extensions_map_t extensions_;
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
					asset_extensions
				 >
		, public utils::singleton<asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>>
	{
		friend class utils::singleton<asset_finder<ExtraAssetsTypes_, UseDefaultTypes_>>;

	public:
		typedef typename std::conditional
		<
			UseDefaultTypes_,
			typename utils::tl::append<DefaultAssetsTypes,ExtraAssetsTypes_>::Result,
			ExtraAssetsTypes_
		>::type AssetsTypeList;

	public:
		template<typename T>
		void registerExtensions(const std::string &extensions);

		template<typename T>
		std::shared_ptr<T> load(const std::string &url, bool loadIfNotFound = true);

	private:
		asset_finder<ExtraAssetsTypes_,UseDefaultTypes_>() {}
		~asset_finder<ExtraAssetsTypes_,UseDefaultTypes_>() {}

		template<typename T>
		std::shared_ptr<T> loadFromUrl(const std::string &url);

		template<typename T>
		void saveToUrl(const T &obj, const std::string &url);

		template<typename T>
		bool hasLoader(const std::string &url);
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	typedef asset_finder<> default_asset_finder;
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/

#include "asset_finder.inl"

#endif /* DJAH_RESOURCES_ASSET_FINDER_HPP */