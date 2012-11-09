#ifndef DJAH_RESOURCES_RESOURCE_MANAGER_HPP
#define DJAH_RESOURCES_RESOURCE_MANAGER_HPP

#include <map>
#include <memory>
#include "../core/singleton.hpp"
#include "resource_base.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	class resource_manager
		: public utils::singleton<resource_manager>
	{
		DJAH_MAKE_SINGLETON(resource_manager);

	public:
		template<typename T>
		std::shared_ptr<T> find(const std::string &name) const;

		void add(const std::string &name, resource_ptr res);
		void remove(const std::string &name);
		void remove(resource_ptr res);
		void cleanUp();

	private:
		typedef std::map<std::string, resource_ptr> resource_map_t;
		resource_map_t resources_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline std::shared_ptr<T> resource_manager::find(const std::string &name) const
	{
		std::shared_ptr<T> res;
		resource_map_t::const_iterator it = resources_.find(name);
		if( it != resources_.end() )
		{
			res = std::shared_ptr<T>(std::dynamic_pointer_cast<T>(it->second));
		}

		return res;
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_RESOURCE_MANAGER_HPP */