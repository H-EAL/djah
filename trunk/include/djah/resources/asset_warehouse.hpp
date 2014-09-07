#ifndef DJAH_RESOURCES_ASSET_WAREHOUSE_HPP
#define DJAH_RESOURCES_ASSET_WAREHOUSE_HPP

#include <map>
#include <memory>
#include "djah/types.hpp"
#include "djah/core/singleton.hpp"
#include "djah/resources/asset.hpp"

namespace djah { namespace resources {

    //----------------------------------------------------------------------------------------------
    class asset_warehouse
        : public utils::singleton<asset_warehouse>
    {
        DJAH_MAKE_SINGLETON(asset_warehouse);

    public:
        struct asset_infos
        {
            asset_wptr wpAsset_;
            u32		   size_;
        };
        typedef std::map<std::string, asset_infos> asset_map_t;

    public:
        template<typename AssetType>
        std::shared_ptr<AssetType> find(const std::string &name) const;

        void add(const std::string &name, asset_sptr spAsset);
        void remove(const std::string &name);
        void remove(asset_sptr res);
        void cleanUp();

        const asset_map_t& assets() const { return assets_; }
        asset_map_t&       assets()       { return assets_; }

    private:
        asset_warehouse();
        ~asset_warehouse();

    private:
        asset_map_t::iterator remove(asset_map_t::const_iterator &it);

    private:
        asset_map_t assets_;
        u64			loadedAssetsTotalWeight_;
    };
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    template<typename AssetType>
    inline std::shared_ptr<AssetType> asset_warehouse::find(const std::string &name) const
    {
        std::shared_ptr<AssetType> spAsset;

        auto it = assets_.find(name);
        if( it != assets_.end() && !it->second.wpAsset_.expired() )
        {
            spAsset = std::shared_ptr<AssetType>(std::dynamic_pointer_cast<AssetType>(it->second.wpAsset_.lock()));
        }

        return spAsset;
    }
    //----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_ASSET_WAREHOUSE_HPP */