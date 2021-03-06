#include "djah/resources/asset.hpp"
#include "djah/resources/asset_warehouse.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	asset::asset(const std::string &_name, u64 _timestamp)
		: name_(_name)
		, timestamp_(_timestamp)
	{

	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	asset::~asset()
	{
		asset_warehouse::get().remove(name_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	const std::string& asset::name() const
	{
		return name_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void asset::setName(const std::string &name)
	{
		name_ = name;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	asset& asset::operator =(const asset &rhs)
	{
		if( this != &rhs )
		{
			name_	   = rhs.name_;
			timestamp_ = rhs.timestamp_;
		}
		return (*this);
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/