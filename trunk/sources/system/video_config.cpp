#include "system/video_config.hpp"
#include "types.hpp"
#include "debug/assertion.hpp"

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	gl_format::gl_format(eOpenglVersion oglVersion, bool _enableDebug)
		: enableDebug(_enableDebug)
	{
		const word version = low_word( (dword)oglVersion );
		majorVersion = high_byte( version );
		minorVersion = low_byte ( version );

		isOpenglES   = high_word( (dword)version ) != 0;
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	gl_format::gl_format(int _majorVersion, int _minorVersion, bool _isOpenglES, bool _enableDebug)
		: majorVersion(_majorVersion)
		, minorVersion(_minorVersion)
		, isOpenglES(_isOpenglES)
		, enableDebug(_enableDebug)
	{
		if( isOpenglES )
		{
			DJAH_ASSERT
			( 
					(majorVersion == 1 || majorVersion == 2)
				&&	(minorVersion == 0 || minorVersion == 1)
			);

			enableDebug = false;
		}
	}
	//----------------------------------------------------------------------------------------------


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

} /*system*/ } /*djah*/
