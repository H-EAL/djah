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
	template<typename AssetsTypeList>
	struct assets_visitor;
	//----------------------------------------------------------------------------------------------
	template<>
	struct assets_visitor<utils::nulltype>
	{
		template<typename AssetFinder>
		static void refresh(AssetFinder &assetFinder);

		template<typename AssetFinder>
		static void refresh(AssetFinder &assetFinder, asset_warehouse::asset_map_t &assets) {}
	};
	//----------------------------------------------------------------------------------------------
	template<typename HeadAssetType, typename TailList>
	struct assets_visitor< utils::typelist<HeadAssetType, TailList> >
	{
		template<typename AssetFinder>
		static void refresh(AssetFinder &assetFinder)
		{
			asset_warehouse::asset_map_t assets = asset_warehouse::get().assets();
			assets_visitor< utils::typelist<HeadAssetType, TailList> >::refresh(assetFinder, assets);
		}

		template<typename AssetFinder>
		static void refresh(AssetFinder &assetFinder, asset_warehouse::asset_map_t &assets)
		{
			auto it = assets.begin();
			while( it != assets.end() )
			{
				if( !it->second.wpAsset_.expired() )
				{
					auto spAsset = std::shared_ptr<HeadAssetType>(std::dynamic_pointer_cast<HeadAssetType>(it->second.wpAsset_.lock()));
					if( spAsset )
					{
						assetFinder.refresh<HeadAssetType>(it->first, spAsset);
						it = assets.erase(it);
					}
					else
					{
						++it;
					}
				}
				else
				{
					it = assets.erase(it);
				}
			}

			assets_visitor<TailList>::refresh(assetFinder, assets);
		}
	};
	//----------------------------------------------------------------------------------------------


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
		template<typename AssetType>
		void registerExtensions(const std::string &extensions);

		template<typename AssetType>
		std::shared_ptr<AssetType> load(const std::string &url, bool loadIfNotFound = true);

		void refreshAll();

		template<typename AssetType>
		void refresh();

		template<typename AssetType>
		bool refresh(const std::string &url, std::shared_ptr<AssetType> &spAsset);

	private:
		asset_finder<ExtraAssetsTypes_,UseDefaultTypes_>() {}
		~asset_finder<ExtraAssetsTypes_,UseDefaultTypes_>() {}

		template<typename AssetType>
		bool loadFromUrl(const std::string &url, std::shared_ptr<AssetType> &spAsset);

		template<typename AssetType>
		void saveToUrl(const AssetType &obj, const std::string &url);

		template<typename AssetType>
		bool hasLoader(const std::string &url);
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	typedef asset_finder<> default_asset_finder;
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/

#include "asset_finder.inl"

#endif /* DJAH_RESOURCES_ASSET_FINDER_HPP */