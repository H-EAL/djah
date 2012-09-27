#include "system/device.hpp"
#include "system/opengl/gl.hpp"
#include "system/opengl/opengl_driver.hpp"


namespace djah { namespace system {
	
	//----------------------------------------------------------------------------------------------
	class device_impl
	{
	public:

		device_impl() : hInstance_(0), hWindow_(0), hDC_(0), hGLRC_(0) {}
		~device_impl() {}

		void createWindow(const video_config &cfg);
		void createContext(const video_config &cfg);

		// Platform specific
		void setupPixelFormat(const video_config &cfg);
		HINSTANCE	hInstance_;		// Application handler
		HWND		hWindow_;		// Window handler	-> to screen
		HDC			hDC_;			// Drawing context	-> to context
		HGLRC		hGLRC_;			// OpenGL context	-> to context

		friend LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		device* dev = device::get_current();

		assert(dev);
		assert(dev->videoDriver());

		switch(msg)
		{	
		case WM_CLOSE:
			dev->shutDown();
			break;
			/*
		case WM_CREATE:
			device->hWindow_ = hWnd;
			device->driver_->create();
			break;*/

		case WM_SIZE:
			dev->resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_KEYUP:
			if(wParam == VK_ESCAPE)
				dev->shutDown();
			break;

		case WM_CHAR:
			// -> buffer this
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device_impl::createWindow(const video_config &cfg)
	{
		hInstance_ = GetModuleHandle(0);
		assert(hInstance_);
		WNDCLASSEXA window_class =
		{
			sizeof(WNDCLASSEX),
			CS_OWNDC | CS_VREDRAW | CS_HREDRAW,
			WinProc, 0, 0, hInstance_,
			0, LoadCursor(0, IDC_ARROW),
			0, 0, "win32_device", 0
		};
		assert( RegisterClassExA(&window_class) );

		// Take into account borders
		int real_width  = cfg.width  + (GetSystemMetrics(SM_CXSIZEFRAME) << 1);
		int real_height = cfg.height + GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYSIZEFRAME) << 1);

		// Center on the screen
		int pos_x = (GetSystemMetrics(SM_CXSCREEN) - real_width)  / 2;
		int pos_y = (GetSystemMetrics(SM_CYSCREEN) - real_height) / 2;

		long style = WS_VISIBLE;
		long ex_style = 0;
		
		if(cfg.fullscreen)
		{
			style |= WS_POPUPWINDOW | WS_MAXIMIZE;
			pos_x = pos_y = 0;
			real_width  = cfg.width;
			real_height = cfg.height;
		}
		else
		{
			style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		}

		hWindow_ = CreateWindowExA
		(
			ex_style,
			"win32_device",
			"Djah's Heavenly Window",
			style,
			pos_x, pos_y,
			real_width, real_height,
			0, 0,
			hInstance_,
			0
		);

		assert(hWindow_);
	}
	//----------------------------------------------------------------------------------------------		


	//----------------------------------------------------------------------------------------------
	void device_impl::createContext(const video_config &cfg)
	{
		hDC_ = GetDC(hWindow_);
		assert(hDC_);

		setupPixelFormat(cfg);
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
			if( wglSwapIntervalEXT ) wglSwapIntervalEXT(cfg.vsync ? 1 : 0);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device_impl::setupPixelFormat(const video_config &cfg)
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
			0,									// Nombre de buffers auxiliaires
			0, 0, 0, 0, 0
		};

		int pixelFormat;
		pixelFormat = ChoosePixelFormat(hDC_, &pfd);
		if (!pixelFormat)
		{
			MessageBoxA(hWindow_, "Mode graphique non supporté", "Problème", MB_ICONERROR | MB_OK);
			exit(1);
		}	
		if (!SetPixelFormat(hDC_, pixelFormat, &pfd))
		{
			MessageBoxA(hWindow_, "Mode graphique non supporté", "Problème", MB_ICONERROR | MB_OK);
			exit(1);
		}	
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/



namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	device::device()
		: hasToQuit_(false)
		, p_driver_(0)
		, p_impl_(new device_impl)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	device::~device()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device::create(const video_config &cfg)
	{
		sp_device_inst_ = this;
		video_config_   = cfg;
		
		p_impl_->createWindow(video_config_);	// -> to screen class
		p_impl_->createContext(video_config_);	// -> to context class
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device::destroy()
	{
		p_driver_->destroy();
		delete p_driver_;
		p_driver_ = 0;
		
		wglMakeCurrent(0,0);
		ReleaseDC(p_impl_->hWindow_, p_impl_->hDC_);
		sp_device_inst_ = 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device::run()
	{
		MSG msg;
		while( PeekMessage(&msg, p_impl_->hWindow_, 0, 0, PM_NOREMOVE) )
		{
			if( GetMessage(&msg, p_impl_->hWindow_, 0, 0) <= 0 )
				return false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		UpdateWindow(p_impl_->hWindow_);

		return !hasToQuit_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device::shutDown()
	{
		hasToQuit_ = true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device::resize(int width, int height)
	{
		p_driver_->setViewport(geometry::rect_i(0,0,width,height));
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	void device::show()
	{
		ShowWindow(p_impl_->hWindow_, SW_SHOW);
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	bool device::isWindowActive() const
	{
		return GetActiveWindow() == p_impl_->hWindow_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device::hasWindowFocus() const
	{
		return GetFocus() == p_impl_->hWindow_;
	}
	//----------------------------------------------------------------------------------------------

		
	//----------------------------------------------------------------------------------------------
	void device::setWindowTitle(const std::string &title)
	{
		SetWindowTextA(p_impl_->hWindow_, title.c_str());
	}
	//----------------------------------------------------------------------------------------------

		
	//----------------------------------------------------------------------------------------------
	void device::swapBuffers()
	{
		SwapBuffers(p_impl_->hDC_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	math::vector2i device::correctedMousePosition(const math::vector2i &absoluteMousePosition) const
	{
		POINT p = {absoluteMousePosition.x, absoluteMousePosition.y};
		ScreenToClient(p_impl_->hWindow_, &p);
		return math::vector2i(p.x, p.y);
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/