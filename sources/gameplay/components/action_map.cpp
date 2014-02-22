#include "djah/gameplay/components/action_map.hpp"

namespace djah { namespace gameplay { namespace components {

	//----------------------------------------------------------------------------------------------
	action_map::action_map(const rapidjson::Value &node)
	{
		load_attributes(actions);

		std::for_each(actions.begin(), actions.end(), [&](const actions_map_t::value_type &entry)
		{
			states[entry.first] = 0.0f;
		});
	}
	//----------------------------------------------------------------------------------------------
	void action_map::serialize(rapidjson::Document &doc) const
	{
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*gameplay*/ } /*djah*/