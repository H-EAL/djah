#ifndef GAME_COMPONENTS_ACTION_MAP_HPP
#define GAME_COMPONENTS_ACTION_MAP_HPP

#include <map>
#include <string>
#include <vector>
#include "djah/gameplay/component.hpp"

namespace game { namespace components {

	struct action_map
	{
		MAKE_COMPONENT(action_map, 1);
		typedef std::vector<std::string> input_sequence_t;
		typedef std::map<std::string, std::vector<input_sequence_t>> actions_map_t;
		typedef std::map<std::string, float> actions_state_t;

		// serializable
		actions_map_t actions;

		// transient
		actions_state_t states;
	};

} /*components*/ } /*game*/

#endif /* GAME_COMPONENTS_ACTION_MAP_HPP */