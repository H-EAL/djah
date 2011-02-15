#include "video/video_config.hpp"

namespace djah { namespace video {

	//----------------------------------------------------------------------------------------------
	video_config::video_config(int _width, int _height, int _colorBits, int _depthBits, int _stencilBits, bool _fullscreen, bool _vsync)
		: width(_width)
		, height(_height)
		, colorBits(_colorBits)
		, depthBits(_depthBits)
		, stencilBits(_stencilBits)
		, fullscreen(_fullscreen)
		, vsync(_vsync)
	{

	}
	//----------------------------------------------------------------------------------------------

} /*video*/ } /*djah*/