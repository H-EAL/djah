#ifndef GAME_COMPONENTS_SCENE_NODE_HPP
#define GAME_COMPONENTS_SCENE_NODE_HPP

#include "djah/gameplay/component.hpp"
#include "djah/math/matrix4.hpp"

namespace game { namespace components {

	struct scene_node
	{
        MAKE_COMPONENT(scene_node, 1000);

		// serializable
        int parentIndex = -1;
        int depth = 0;
        
        // transient
        djah::math::matrix4f in_World;
	};

} /*components*/ } /*game*/

#endif /* GAME_COMPONENTS_SCENE_NODE_HPP */