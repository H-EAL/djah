#include <cassert>
#include "system/opengl_include.hpp"

#include "system/opengl_driver.hpp"
#include "system/windows/opengl_win32.hpp"
#include "system/windows/win32device.hpp"

namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	opengl_context* new_platform_specific_context()
	{
		return new opengl_win32;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	opengl_win32::opengl_win32()
		: hDC_(0)
		, hGLRC_(0)
		, hWindow_(0)
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
		win32device *w32dev = dynamic_cast<win32device*>(device);
		assert(w32dev);

		hWindow_ = w32dev->hWindow_;
		hDC_ = GetDC(hWindow_);
		assert(hDC_);

		setupPixelFormat(device->videoConfig());
		hGLRC_ = wglCreateContext(hDC_);
		assert(hGLRC_);

		makeCurrent();

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
		doneCurrent();
		ReleaseDC(hWindow_, hDC_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_win32::swapBuffers()
	{
		SwapBuffers(hDC_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_win32::makeCurrent()
	{
		wglMakeCurrent(hDC_, hGLRC_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_win32::doneCurrent()
	{
		wglMakeCurrent(0,0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void opengl_win32::setupPixelFormat(const video_config &cfg)
	{
		PIXELFORMATDESCRIPTOR pfd =
		{	 
			sizeof(PIXELFORMATDESCRIPTOR), 1,
			PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			static_cast<BYTE>(cfg.colorBits),	// Bits de couleur
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
			static_cast<BYTE>(cfg.depthBits),	// Bits de profondeur
			static_cast<BYTE>(cfg.stencilBits),	// Bits du buffer stencil
			0,														// Nombre de buffers auxiliaires
			0, 0, 0, 0, 0
		};	 

		int pixelFormat;
		pixelFormat = ChoosePixelFormat(hDC_, &pfd);
		if (!pixelFormat)
		{
			MessageBoxA(WindowFromDC(hDC_), "Mode graphique non supporté", "Problème", MB_ICONERROR | MB_OK);
			exit(1);
		}	
		if (!SetPixelFormat(hDC_, pixelFormat, &pfd))
		{
			MessageBoxA(WindowFromDC(hDC_), "Mode graphique non supporté", "Problème", MB_ICONERROR | MB_OK);
			exit(1);
		}	
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/
