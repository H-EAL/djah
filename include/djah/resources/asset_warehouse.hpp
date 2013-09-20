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
		template<typename T>
		std::shared_ptr<T> find(const std::string &name) const;

		void add(const std::string &name, asset_sptr res);
		void remove(const std::string &name);
		void remove(asset_sptr res);
		void cleanUp();

	private:
		asset_warehouse();

	private:
		typedef std::map<std::string, asset_wptr> asset_map_t;

		asset_map_t::iterator remove(asset_map_t::const_iterator &it);

	private:
		asset_map_t assets_;
		u64			loadedAssetsTotalWeight_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline std::shared_ptr<T> asset_warehouse::find(const std::string &name) const
	{
		std::shared_ptr<T> pAsset = nullptr;
		auto it = assets_.find(name);
		if( it != assets_.end() && !it->second.expired() )
		{
			pAsset = std::shared_ptr<T>(std::dynamic_pointer_cast<T>(it->second.lock()));
			check(pAsset);
		}
		return pAsset;
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_ASSET_WAREHOUSE_HPP */