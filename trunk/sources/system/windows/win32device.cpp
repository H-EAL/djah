#include "system/windows/win32device.hpp"
#include "system/opengl_include.hpp"

#include <cassert>

#include "geometry/rect.hpp"


namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	device_ptr new_platform_specific_device()
	{
		return new win32device;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		win32device* device = dynamic_cast<win32device*>(device_base::get_current());

		assert(device);
		assert(device->driver_);

		switch(msg)
		{	
		case WM_CLOSE:
			device->shutDown();
			break;
			/*
		case WM_CREATE:
			device->hWindow_ = hWnd;
			device->driver_->create();
			break;*/

		case WM_SIZE:
			device->resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_KEYUP:
			if(wParam == VK_ESCAPE)
				device->shutDown();
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	win32device::win32device()
		: hInstance_(0)
		, hWindow_(0)
		, hDC_(0)
		, hGLRC_(0)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	win32device::~win32device()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void win32device::createImpl()
	{
		hInstance_ = GetModuleHandle(0);
		assert(hInstance_);
		WNDCLASSEXA window_class =
		{
			sizeof(WNDCLASSEX),
			CS_OWNDC | CS_VREDRAW | CS_HREDRAW,
			WinProc, 0, 0, hInstance_,
			0, LoadCursor(0, IDC_ARROW),
			0, 0, "win32device", 0
		};
		assert( RegisterClassExA(&window_class) );

		// Take into account borders
		int real_width  = video_config_.width  + (GetSystemMetrics(SM_CXSIZEFRAME) << 1);
		int real_height = video_config_.height + GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYSIZEFRAME) << 1);

		// Center on the screen
		int pos_x = (GetSystemMetrics(SM_CXSCREEN) - real_width)  / 2;
		int pos_y = (GetSystemMetrics(SM_CYSCREEN) - real_height) / 2;

		long style = WS_VISIBLE;
		long ex_style = 0;
		
		if(video_config_.fullscreen)
		{
			style |= WS_POPUPWINDOW | WS_MAXIMIZE;
			pos_x = pos_y = 0;
			real_width  = video_config_.width;
			real_height = video_config_.height;
		}
		else
		{
			ex_style |= WS_EX_OVERLAPPEDWINDOW;
			style    |= WS_OVERLAPPEDWINDOW;
		}

		hWindow_ = CreateWindowExA
		(
			ex_style,
			"win32device",
			"Djah's Heavenly Window",
			style,
			pos_x, pos_y,
			real_width, real_height,
			0, 0,
			hInstance_,
			0
		);

		assert(hWindow_);

		hDC_ = GetDC(hWindow_);
		assert(hDC_);

		setupPixelFormat(video_config_);
		hGLRC_ = wglCreateContext(hDC_);
		assert(hGLRC_);
		
		wglMakeCurrent(hDC_, hGLRC_);

		// Handle VSync
		typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
		PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
		const char *extensions = reinterpret_cast<const char*>(glGetString( GL_EXTENSIONS ));
		if( strstr( extensions, "WGL_EXT_swap_control" ) != 0 )
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
			if( wglSwapIntervalEXT ) wglSwapIntervalEXT(video_config_.vsync ? 1 : 0);
		}

		UpdateWindow(hWindow_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void win32device::destroyImpl()
	{
		wglMakeCurrent(0,0);
		ReleaseDC(hWindow_, hDC_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void win32device::show()
	{
		ShowWindow(hWindow_, SW_SHOW);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool win32device::isWindowActive()
	{
		return GetActiveWindow() == hWindow_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool win32device::hasWindowFocus()
	{
		return GetFocus() == hWindow_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void win32device::setWindowTitle(const std::string &title)
	{
		SetWindowTextA(hWindow_, title.c_str());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void win32device::swapBuffers()
	{
		SwapBuffers(hDC_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool win32device::runImpl()
	{
		MSG msg;
		while( PeekMessage(&msg, hWindow_, 0, 0, PM_NOREMOVE) )
		{
			if( GetMessage(&msg, hWindow_, 0, 0) <= 0 )
				return false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		UpdateWindow(hWindow_);

		return true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void win32device::setupPixelFormat(const video_config &cfg)
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
			MessageBoxA(WindowFromDC(hDC_), "Mode graphique non support�", "Probl�me", MB_ICONERROR | MB_OK);
			exit(1);
		}	
		if (!SetPixelFormat(hDC_, pixelFormat, &pfd))
		{
			MessageBoxA(WindowFromDC(hDC_), "Mode graphique non support�", "Probl�me", MB_ICONERROR | MB_OK);
			exit(1);
		}	
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/