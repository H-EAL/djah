#include "djah/gameplay/components/action_map.hpp"

namespace djah { namespace gameplay { namespace components {

	//----------------------------------------------------------------------------------------------
	action_map::action_map(const rapidjson::Value &node)
	{
		JSON_READ(actions);

		std::for_each(actions.begin(), actions.end(), [&](const actions_map_t::value_type &entry)
		{
			states[entry.first] = false;
		});
	}
	//----------------------------------------------------------------------------------------------
	void action_map::serialize(djah::filesystem::stream &strm) const
	{
	}
	//----------------------------------------------------------------------------------------------

} /*components*/ } /*gameplay*/ } /*djah*/