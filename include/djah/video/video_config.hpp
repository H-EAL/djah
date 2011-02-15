#ifndef DJAH_VIDEO_VIDEO_CONFIG_HPP
#define DJAH_VIDEO_VIDEO_CONFIG_HPP

namespace djah { namespace video {

	class video_config
	{
	public:

		video_config(int _width = 640, int _height = 480,
					 int _colorBits = 32, int _depthBit = 24, int _stencilBits = 0,
					 bool _fullscreen = false, bool _vsync = true);

		int	 width;
		int  height;
		int  colorBits;
		int  depthBits;
		int  stencilBits;
		bool fullscreen;
		bool vsync;
	};

} /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_VIDEO_CONFIG_HPP */