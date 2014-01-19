#ifndef DJAH_GAMEPLAY_COMPONENTS_TRANSFORM_HPP
#define DJAH_GAMEPLAY_COMPONENTS_TRANSFORM_HPP

#include "djah/gameplay/components/component.hpp"
#include "djah/math/vector3.hpp"
#include "djah/math/quaternion.hpp"

namespace djah { namespace gameplay { namespace components {

	struct transform
	{
		MAKE_COMPONENT(transform);

		// serializable
		math::vector3f position;
		math::quatf    orientation;
		math::vector3f scale;

		// transient
	};

} /*components*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_TRANSFORM_HPP */