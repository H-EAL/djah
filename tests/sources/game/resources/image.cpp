#include "game/resources/image.hpp"

namespace game { namespace resources {

	//----------------------------------------------------------------------------------------------
    image::image(djah::u32 _width, djah::u32 _height, djah::u32 _channels, const djah::byte *pPixels)
		: width_(_width)
		, height_(_height)
		, channels_(_channels)
		, pPixels_(_width * _height * _channels)
	{
		std::copy(pPixels, pPixels + pPixels_.size(), pPixels_.begin());
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*game*/