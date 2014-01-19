#ifndef DJAH_GAMEPLAY_COMPONENTS_HPP
#define DJAH_GAMEPLAY_COMPONENTS_HPP

#include "djah/core/typelist.hpp"

#include "djah/gameplay/components/transform.hpp"
#include "djah/gameplay/components/fov.hpp"
#include "djah/gameplay/components/visual_mesh.hpp"
#include "djah/gameplay/components/texture.hpp"
#include "djah/gameplay/components/action_map.hpp"

namespace djah { namespace gameplay { namespace components {

	typedef TYPELIST
	(
		  transform
		, fov
		, visual_mesh
		, texture
		, action_map
	)
	DefaultComponentTypes;

} /*components*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_HPP */