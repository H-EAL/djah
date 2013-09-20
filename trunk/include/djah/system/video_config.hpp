#ifndef DJAH_SYSTEM_VIDEO_CONFIG_HPP
#define DJAH_SYSTEM_VIDEO_CONFIG_HPP

#include <string>
#include <memory>

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	enum eOpenglVersion
	{
		OpenGL_Version_None		= -1,
		OpenGL_Version_Default	= 0,

		OpenGL_Version_1_0		= (1 << 8) | 0,
		OpenGL_Version_1_1		= (1 << 8) | 1,
		OpenGL_Version_1_2		= (1 << 8) | 2,
		OpenGL_Version_1_3		= (1 << 8) | 3,
		OpenGL_Version_1_4		= (1 << 8) | 4,
		OpenGL_Version_1_5		= (1 << 8) | 5,

		OpenGL_Version_2_0		= (2 << 8) | 0,
		OpenGL_Version_2_1		= (2 << 8) | 1,

		OpenGL_Version_3_0		= (3 << 8) | 0,
		OpenGL_Version_3_1		= (3 << 8) | 1,
		OpenGL_Version_3_2		= (3 << 8) | 2,
		OpenGL_Version_3_3		= (3 << 8) | 3,

		OpenGL_Version_4_0		= (4 << 8) | 0,
		OpenGL_Version_4_1		= (4 << 8) | 1,
		OpenGL_Version_4_2		= (4 << 8) | 2,
		OpenGL_Version_4_3		= (4 << 8) | 3,

		OpenGL_ES_Version_1_0	= (1 << 16) | (1 << 8) | 0,
		OpenGL_ES_Version_1_1	= (1 << 16) | (1 << 8) | 1,
		OpenGL_ES_Version_2_0	= (1 << 16) | (2 << 8) | 0,
		OpenGL_ES_Version_2_1	= (1 << 16) | (2 << 8) | 1,
	};
	//----------------------------------------------------------------------------------------------
	class driver_config
	{
	public:
		driver_config(eOpenglVersion version = OpenGL_Version_Default, bool _enableDebug = false,
					  bool _enableCompatibilityProfile = true, bool _vsync = true);

		driver_config(int _majorVersion, int _minorVersion, bool _isOpenglES,
					  bool _enableDebug, bool _enableCompatibilityProfile, bool _vsync);

		int  majorVersion;
		int  minorVersion;
		bool isOpenglES;
		bool enableDebug;
		bool enableCompatibilityProfile;
		bool vsync;

	public:
		static int s_default_major_version;
		static int s_default_minor_version;
	};
	typedef std::shared_ptr<driver_config> driver_config_sptr;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class device_config
	{
	public:
		device_config(int _width = 640, int _height = 480, int _colorBits = 32, int _depthBit = 24,
					  int _stencilBits = 0, bool _fullscreen = false,
					  const std::string &_title = "Djah's Heavenly Window");
		virtual ~device_config() {}

		int width;
		int height;
		int colorBits;
		int depthBits;
		int stencilBits;
		bool fullscreen;
		std::string title;
	};
	typedef std::shared_ptr<device_config> device_config_sptr;
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_VIDEO_CONFIG_HPP */