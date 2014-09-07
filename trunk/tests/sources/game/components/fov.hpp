#ifndef GAME_COMPONENTS_FOV_HPP
#define GAME_COMPONENTS_FOV_HPP

#include "djah/gameplay/component.hpp"

namespace game { namespace components {

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

} /*components*/ } /*game*/

#endif /* GAME_COMPONENTS_FOV_HPP */