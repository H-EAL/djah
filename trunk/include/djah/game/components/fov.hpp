#ifndef DJAH_GAME_COMPONENTS_FOV_HPP
#define DJAH_GAME_COMPONENTS_FOV_HPP

#include "djah/gameplay/component.hpp"

namespace djah { namespace game { namespace components {

	struct fov
	{
		MAKE_COMPONENT(fov, 10);

		// serializable
		float horizontal;
		float vertical;
		float nearPlane;
		float farPlane;
		float distortion;
	};

} /*components*/ } /*game*/ } /*djah*/

#endif /* DJAH_GAME_COMPONENTS_FOV_HPP */