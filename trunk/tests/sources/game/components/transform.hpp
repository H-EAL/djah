#ifndef GAME_COMPONENTS_TRANSFORM_HPP
#define GAME_COMPONENTS_TRANSFORM_HPP

#include "djah/gameplay/component.hpp"
#include "djah/math/vector3.hpp"
#include "djah/math/quaternion.hpp"

namespace game { namespace components {

	struct transform
	{
		MAKE_COMPONENT(transform);

		// serializable
        djah::math::vector3f position;
        djah::math::quatf    orientation;
        djah::math::vector3f scale;

		// transient

		// Helper functions
        djah::math::matrix4f toMatrix4() const;
	};

} /*components*/ } /*game*/

#endif /* GAME_COMPONENTS_TRANSFORM_HPP */