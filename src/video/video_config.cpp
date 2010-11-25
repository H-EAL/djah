#include "video/video_config.hpp"

namespace djah { namespace video {

	//----------------------------------------------------------------------------------------------
	VideoConfig::VideoConfig(int width, int height, int colorBits, int depthBits, int stencilBits, bool fullscreen, bool vsync)
		: width_(width)
		, height_(height)
		, colorBits_(colorBits)
		, depthBits_(depthBits)
		, stencilBits_(stencilBits)
		, fullscreen_(fullscreen)
		, vsync_(vsync)
	{

	}
	//----------------------------------------------------------------------------------------------

} /*video*/ } /*djah*/