#include "resources/resource_manager.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	void resource_manager::add(const std::string &name, resource_ptr res)
	{
		resource_map_t::const_iterator it = resources_.find(name);
		if( it == resources_.end() )
		{
			res->setName(name);
			resources_.insert( resource_map_t::value_type(name, res.get()) );
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void resource_manager::remove(const std::string &name)
	{
		resource_map_t::iterator it = resources_.find(name);
		if( it != resources_.end() )
		{
			resources_.erase(it);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void resource_manager::remove(resource_ptr res)
	{
		if( res )
			remove(res->name());
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/