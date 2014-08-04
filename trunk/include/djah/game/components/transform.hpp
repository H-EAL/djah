#ifndef DJAH_GAME_COMPONENTS_TRANSFORM_HPP
#define DJAH_GAME_COMPONENTS_TRANSFORM_HPP

#include "djah/gameplay/component.hpp"
#include "djah/math/vector3.hpp"
#include "djah/math/quaternion.hpp"

namespace djah { namespace game { namespace components {

	struct transform
	{
		MAKE_COMPONENT(transform);

		// serializable
		math::vector3f position;
		math::quatf    orientation;
		math::vector3f scale;

		// transient
	};

} /*components*/ } /*game*/ } /*djah*/

#endif /* DJAH_GAME_COMPONENTS_TRANSFORM_HPP */