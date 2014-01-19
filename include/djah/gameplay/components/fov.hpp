#ifndef DJAH_GAMEPLAY_COMPONENTS_FOV_HPP
#define DJAH_GAMEPLAY_COMPONENTS_FOV_HPP

#include "djah/gameplay/components/component.hpp"

namespace djah { namespace gameplay { namespace components {

	struct fov
	{
		MAKE_COMPONENT(fov);

		// serializable
		float horizontal;
		float vertical;
		float nearPlane;
		float farPlane;
		float distortion;
	};

} /*components*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_FOV_HPP */