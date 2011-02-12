#ifndef DJAH_VIDEO_VIDEO_CONFIG_HPP
#define DJAH_VIDEO_VIDEO_CONFIG_HPP

namespace djah { namespace video {

	class video_config
	{
	public:

		video_config(int width = 640, int height = 480,
					 int colorBits = 32, int depthBit = 24, int stencilBits = 0,
					 bool fullscreen = false, bool vsync = true);

		int	 width_;
		int  height_;
		int  colorBits_;
		int  depthBits_;
		int  stencilBits_;
		bool fullscreen_;
		bool vsync_;
	};

} /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_VIDEO_CONFIG_HPP */