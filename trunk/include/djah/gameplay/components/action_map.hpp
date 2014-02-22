#ifndef DJAH_GAMEPLAY_COMPONENTS_ACTION_MAP_HPP
#define DJAH_GAMEPLAY_COMPONENTS_ACTION_MAP_HPP

#include <map>
#include <string>
#include <vector>
#include "djah/gameplay/components/component.hpp"

namespace djah { namespace gameplay { namespace components {

	struct action_map
	{
		MAKE_COMPONENT(action_map);
		typedef std::vector<std::string> input_sequence_t;
		typedef std::map<std::string, std::vector<input_sequence_t>> actions_map_t;
		typedef std::map<std::string, float> actions_state_t;

		// serializable
		actions_map_t actions;

		// transient
		actions_state_t states;
	};

} /*components*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_ACTION_MAP_HPP */