#include "djah/resources/image.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	image::image(u32 _width, u32 _height, u32 _channels, const byte *pPixels)
		: width_(_width)
		, height_(_height)
		, channels_(_channels)
		, pPixels_(_width * _height * _channels)
	{
		std::copy(pPixels, pPixels + pPixels_.size(), pPixels_.begin());
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/