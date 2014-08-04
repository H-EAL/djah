#include "djah/system/video_config.hpp"
#include "djah/types.hpp"
#include "djah/debug/assertion.hpp"

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	int driver_config::s_default_major_version = -1;
	int driver_config::s_default_minor_version = -1;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	driver_config::driver_config(eOpenglVersion oglVersion, bool _enableDebug,
								 bool _enableCompatibilityProfile, bool _vsync)
		: enableDebug(_enableDebug)
		, enableCompatibilityProfile(_enableCompatibilityProfile)
		, vsync(_vsync)
	{
		const word version	= low_word( (dword)oglVersion );
		majorVersion		= high_byte( version );
		minorVersion		= low_byte ( version );
		isOpenglES			= high_word( (dword)oglVersion ) != 0;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	driver_config::driver_config(int _majorVersion, int _minorVersion, bool _isOpenglES,
								 bool _enableDebug, bool _enableCompatibilityProfile, bool _vsync)
		: majorVersion(_majorVersion)
		, minorVersion(_minorVersion)
		, isOpenglES(_isOpenglES)
		, enableDebug(_enableDebug)
		, enableCompatibilityProfile(_enableCompatibilityProfile)
		, vsync(_vsync)
	{
		if( isOpenglES )
		{
			check
			( 
					(majorVersion == 1 || majorVersion == 2)
				&&	(minorVersion == 0 || minorVersion == 1)
			);

			enableDebug = false;
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	device_config::device_config(int _width, int _height, int _colorBits, int _depthBits,
								 int _stencilBits, bool _fullscreen, const std::string &_title)
		: width(_width)
		, height(_height)
		, colorBits(_colorBits)
		, depthBits(_depthBits)
		, stencilBits(_stencilBits)
		, fullscreen(_fullscreen)
		, title(_title)
	{

	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/
