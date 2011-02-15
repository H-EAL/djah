#include <cassert>
#include "video/drivers/opengl_include.hpp"

#include "video/drivers/opengl_base.hpp"
#include "video/drivers/detail/opengl_win32.hpp"
#include "video/devices/win32device.hpp"

namespace djah { namespace video {

	//----------------------------------------------------------------------------------------------
	driver_ptr new_platform_specific_driver(device_base *device)
	{
		return new drivers::opengl_base<drivers::detail::opengl_win32>(device);
	}
	//----------------------------------------------------------------------------------------------

} /*video*/ } /*djah*/

namespace djah { namespace video { namespace drivers { namespace detail {

	//----------------------------------------------------------------------------------------------
	opengl_win32::opengl_win32()
		: hGLRC_(0)
	{
	}
	//----------------------------------------------------------------------------------------------
	

	//----------------------------------------------------------------------------------------------
	opengl_win32::~opengl_win32()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_win32::create(device_ptr device)
	{
		devices::win32device *w32dev = dynamic_cast<devices::win32device*>(device);

		assert(w32dev);
		w32dev->hDC_ = GetDC(w32dev->hWindow_);
		assert(w32dev->hDC_);
		setupPixelFormat(w32dev);
		hGLRC_ = wglCreateContext(w32dev->hDC_);
		assert(hGLRC_);
		wglMakeCurrent(w32dev->hDC_, hGLRC_);

		// Handle VSync
		typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
		PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
		const char *extensions = reinterpret_cast<const char*>(glGetString( GL_EXTENSIONS ));
		if( strstr( extensions, "WGL_EXT_swap_control" ) != 0 )
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
			if( wglSwapIntervalEXT ) wglSwapIntervalEXT(device->videoConfig().vsync ? 1 : 0);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_win32::destroy()
	{
		wglMakeCurrent(0,0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_win32::setupPixelFormat(devices::win32device *device)
	{
		PIXELFORMATDESCRIPTOR pfd =
		{	 
			sizeof(PIXELFORMATDESCRIPTOR), 1,
			PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			static_cast<BYTE>(device->videoConfig().colorBits),	// Bits de couleur
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
			static_cast<BYTE>(device->videoConfig().depthBits),	// Bits de profondeur
			static_cast<BYTE>(device->videoConfig().stencilBits),	// Bits du buffer stencil
			0,														// Nombre de buffers auxiliaires
			0, 0, 0, 0, 0
		};	 

		int pixelFormat;
		pixelFormat = ChoosePixelFormat(device->hDC_, &pfd);
		if (!pixelFormat)
		{
			MessageBoxA(WindowFromDC(device->hDC_), "Mode graphique non supporté", "Problème", MB_ICONERROR | MB_OK);
			exit(1);
		}	
		if (!SetPixelFormat(device->hDC_, pixelFormat, &pfd))
		{
			MessageBoxA(WindowFromDC(device->hDC_), "Mode graphique non supporté", "Problème", MB_ICONERROR | MB_OK);
			exit(1);
		}	
	}
	//----------------------------------------------------------------------------------------------

} /*detail*/ } /*drivers*/ } /*video*/ } /*djah*/