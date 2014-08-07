#ifndef DJAH_GAME_COMPONENTS_REGISTRY_HPP
#define DJAH_GAME_COMPONENTS_REGISTRY_HPP

#include "djah/core/typelist.hpp"

#include "djah/game/components/transform.hpp"
#include "djah/game/components/fov.hpp"
#include "djah/game/components/visual_mesh.hpp"
#include "djah/game/components/texture.hpp"
#include "djah/game/components/action_map.hpp"

namespace djah { namespace game { namespace components {

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

} /*components*/ } /*game*/ } /*djah*/

#endif /* DJAH_GAME_COMPONENTS_REGISTRY_HPP */