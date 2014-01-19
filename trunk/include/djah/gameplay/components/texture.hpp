#ifndef DJAH_GAMEPLAY_COMPONENTS_TEXTURE_HPP
#define DJAH_GAMEPLAY_COMPONENTS_TEXTURE_HPP

#include "djah/gameplay/components/component.hpp"
#include "djah/resources/image.hpp"

namespace djah { namespace gameplay { namespace components {

	struct texture
	{
		MAKE_COMPONENT(texture);

		// serializable
		std::string file;

		// transient
		resources::image_sptr spImage;
	};

} /*components*/ } /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENTS_TEXTURE_HPP */