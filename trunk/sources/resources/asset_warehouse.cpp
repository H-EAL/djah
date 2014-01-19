#include "djah/resources/asset_warehouse.hpp"
#include "djah/debug/log.hpp"
#include "djah/debug/assertion.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	asset_warehouse::asset_warehouse()
		: loadedAssetsTotalWeight_(0)
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void asset_warehouse::add(const std::string &name, asset_sptr pAsset)
	{
		auto it = assets_.find(name);
		if( ensure(it == assets_.end() || it->second.expired()) )
		{
			pAsset->setName(name);
			loadedAssetsTotalWeight_ += pAsset->size();
			assets_.insert( asset_map_t::value_type(name, asset_wptr(pAsset)) );
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	asset_warehouse::asset_map_t::iterator asset_warehouse::remove(asset_map_t::const_iterator &it)
	{
		check(it != assets_.end());
		return assets_.erase(it);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void asset_warehouse::remove(const std::string &name)
	{
		auto it = assets_.find(name);
		if( it != assets_.end() )
		{
			remove(it);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void asset_warehouse::remove(asset_sptr pAsset)
	{
		if( pAsset )
		{
			loadedAssetsTotalWeight_ -= pAsset->size();
			remove(pAsset->name());
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void asset_warehouse::cleanUp()
	{
		auto it    = assets_.begin();
		auto itEnd = assets_.end();
		while(it != itEnd)
		{
			if( it->second.expired() )
			{
				it = remove(it);
			}
			else
			{
				++it;
			}
		}
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/