#ifndef DJAH_GAME_COMPONENTS_TEXTURE_HPP
#define DJAH_GAME_COMPONENTS_TEXTURE_HPP

#include "djah/gameplay/component.hpp"
#include "djah/resources/image.hpp"

namespace djah { namespace game { namespace components {

	struct texture
	{
		MAKE_COMPONENT(texture);

		// serializable
		std::string file;

		// transient
		resources::image_sptr spImage;
	};

} /*components*/ } /*game*/ } /*djah*/

#endif /* DJAH_GAME_COMPONENTS_TEXTURE_HPP */