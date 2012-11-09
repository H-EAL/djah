#ifndef DJAH_SYSTEM_OPENGL_WGL_EXTENSIONS_HPP
#define DJAH_SYSTEM_OPENGL_WGL_EXTENSIONS_HPP

//--------------------------------------------------------------------------------------------------
#ifdef DEFINE_EXTENSIONS_AS_LOCAL
#	define EXTERN2
#else
#	define EXTERN2 extern
#endif

#define DEFINE_EXTENSION(PFN, EXT) EXTERN2 PFN EXT
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
namespace djah { namespace system {

	void load_wgl_extensions();

} /*system*/ } /*djah*/
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
DEFINE_EXTENSION( PFNWGLCREATECONTEXTATTRIBSARBPROC,	wglCreateContextAttribsARB);
DEFINE_EXTENSION( PFNWGLGETEXTENSIONSSTRINGARBPROC,		wglGetExtensionsStringARB);
DEFINE_EXTENSION( PFNWGLSWAPINTERVALEXTPROC,			wglSwapIntervalEXT);
//--------------------------------------------------------------------------------------------------

#endif /* DJAH_SYSTEM_OPENGL_WGL_EXTENSIONS_HPP */