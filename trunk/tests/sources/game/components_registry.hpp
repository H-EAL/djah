#ifndef GAME_COMPONENTS_REGISTRY_HPP
#define GAME_COMPONENTS_REGISTRY_HPP

#include "djah/core/typelist.hpp"

#include "game/components/transform.hpp"
#include "game/components/fov.hpp"
#include "game/components/visual_mesh.hpp"
#include "game/components/texture.hpp"
#include "game/components/action_map.hpp"

namespace game { namespace components {

	typedef TYPELIST
	(
		  transform
		, fov
		, visual_mesh
		, texture
		, uv_modifier
		, action_map
	)
	DefaultComponentTypes;

} /*components*/ } /*game*/

#endif /* GAME_COMPONENTS_REGISTRY_HPP */