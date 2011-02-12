#ifndef DJAH_RESOURCES_RESOURCE_MANAGER_HPP
#define DJAH_RESOURCES_RESOURCE_MANAGER_HPP

#include <map>
#include "../utils/singleton.hpp"
#include "resource_base.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	class resource_manager
		: public utils::singleton<resource_manager>
	{
		DJAH_MAKE_SINGLETON(resource_manager);

	public:
		template<typename T>
		boost::shared_ptr<T> find(const std::string &name) const;

		void add(const std::string &name, resource_ptr res);
		void remove(const std::string &name);

	private:
		typedef std::map<std::string, resource_base*> resource_map_t;
		resource_map_t resources_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline boost::shared_ptr<T> resource_manager::find(const std::string &name) const
	{
		boost::shared_ptr<T> res;
		resource_map_t::const_iterator it = resources_.find(name);
		if( it != resources_.end() )
		{
			res = boost::shared_ptr<T>(static_cast<T*>(it->second));
		}

		return res;
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_RESOURCE_MANAGER_HPP */