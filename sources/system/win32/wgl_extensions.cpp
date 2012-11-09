#include "debug/assertion.hpp"
#include "platform.hpp"
#include "system/gl.hpp"

#define DEFINE_EXTENSIONS_AS_LOCAL
#	include "wgl_extensions.hpp"
#undef DEFINE_EXTENSIONS_AS_LOCAL

namespace {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	inline void load_wgl_extension(T& func, const char *name)
	{
		func = reinterpret_cast<T>( wglGetProcAddress(name) );
		DJAH_ASSERT_MSG(func != 0, "Unable to load WGL extension");
	}
	//----------------------------------------------------------------------------------------------

}

//--------------------------------------------------------------------------------------------------
#define LOAD_EXTENSION(EXT) load_wgl_extension(EXT, #EXT)
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
namespace djah { namespace system {

	void load_wgl_extensions()
	{
		LOAD_EXTENSION( wglCreateContextAttribsARB);
		LOAD_EXTENSION( wglGetExtensionsStringARB);
		LOAD_EXTENSION( wglSwapIntervalEXT);
	}

} /*system*/ } /*djah*/
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
#undef LOAD_EXTENSION
//--------------------------------------------------------------------------------------------------