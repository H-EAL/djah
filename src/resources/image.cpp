#include "resources/image.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	image::image(unsigned int width, unsigned int height, const byte *pixels)
		: width_(width)
		, height_(height)
		, pixels_(width * height * 3)
	{
		std::copy(pixels, pixels + width * height * 3, pixels_.begin());
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/