#include "djah/system/device.hpp"
#include "djah/system/system_logger.hpp"
#include "djah/system/gl.hpp"
#include "djah/system/opengl_driver.hpp"
#include "djah/system/context.hpp"
#include "djah/debug/assertion.hpp"
#include "./wgl_extensions.hpp"


namespace djah { namespace system {
	
	//----------------------------------------------------------------------------------------------
	class device_impl
	{
	public:

		device_impl() : hInstance_(0), hWindow_(0), hDC_(0) {}
		~device_impl() {}

		void createWindow(const video_config &cfg);
		void createContext(const video_config &cfg);

		// Platform specific
		void setupPixelFormat(const video_config &cfg);
		HINSTANCE	hInstance_;		// Application handler
		HWND		hWindow_;		// Window handler
		HDC			hDC_;			// Device Context handler
		HGLRC		hGLRC_;			// Temporary opengl context

		friend LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		device* dev = device::get_current();

		assert(dev);

		switch(msg)
		{	
		case WM_CLOSE:
			dev->shutDown();
			break;

		case WM_SIZE:
			dev->resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_CREATE:
			DJAH_SYSTEM_NOTIFICATION() << "Received WM_CREATE message" << DJAH_END_LOG();
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
		int real_width  = cfg.width  + (GetSystemMetrics(SM_CXFIXEDFRAME) * 2);
		int real_height = cfg.height + GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYFIXEDFRAME) * 2);

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
		DJAH_ASSERT(hDC_ != nullptr);

		setupPixelFormat(cfg);
		hGLRC_ = wglCreateContext(hDC_);
		DJAH_ASSERT(hGLRC_ != nullptr);
		
		wglMakeCurrent(hDC_, hGLRC_);
		load_wgl_extensions();

		/*
		wglDeleteContext(hGLRC_);
		*/
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

		const int pixelFormat = ChoosePixelFormat(hDC_, &pfd);
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
		, pDriver_(0)
		, pImpl_(new device_impl)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	device::~device()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void* device::windowHandle() const
	{
		return reinterpret_cast<void*>( pImpl_->hWindow_ );
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device::create(const video_config &cfg)
	{
		sp_device_inst_ = this;
		videoConfig_    = cfg;
		
		pImpl_->createWindow(videoConfig_);
		pImpl_->createContext(videoConfig_);	// -> to context class

		setVSync( cfg.vsync );
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device::destroy()
	{
		pDriver_->destroy();
		delete pDriver_;
		pDriver_ = nullptr;
		
		gl_context::done_current();

		ReleaseDC(pImpl_->hWindow_, pImpl_->hDC_);

		sp_device_inst_ = 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device::run()
	{
		MSG msg;
		while( PeekMessage(&msg, pImpl_->hWindow_, 0, 0, PM_NOREMOVE) )
		{
			if( GetMessage(&msg, pImpl_->hWindow_, 0, 0) <= 0 )
				return false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		UpdateWindow(pImpl_->hWindow_);

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
		pDriver_->setViewport(geometry::rect_i(0,0,width,height));
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	void device::show()
	{
		ShowWindow(pImpl_->hWindow_, SW_SHOW);
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	bool device::isWindowActive() const
	{
		return GetActiveWindow() == pImpl_->hWindow_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device::hasWindowFocus() const
	{
		return GetFocus() == pImpl_->hWindow_;
	}
	//----------------------------------------------------------------------------------------------

		
	//----------------------------------------------------------------------------------------------
	void device::setWindowTitle(const std::string &title)
	{
		SetWindowTextA(pImpl_->hWindow_, title.c_str());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device::setVSync(bool enabled)
	{
		wglSwapIntervalEXT(enabled ? 1 : 0);
	}
	//----------------------------------------------------------------------------------------------
	void device::enableVSync()
	{
		setVSync(true);
	}
	//----------------------------------------------------------------------------------------------
	void device::disableVSync()
	{
		setVSync(false);
	}
	//----------------------------------------------------------------------------------------------

		
	//----------------------------------------------------------------------------------------------
	void device::swapBuffers()
	{
		SwapBuffers(pImpl_->hDC_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	math::vector2i device::clientMousePosition(const math::vector2i &screenMousePos) const
	{
		POINT ptScreen = {screenMousePos.x, screenMousePos.y};
		ScreenToClient(pImpl_->hWindow_, &ptScreen);
		return math::vector2i(ptScreen.x, ptScreen.y);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	math::vector2i device::screenMousePosition(const math::vector2i &clientMousePos) const
	{
		POINT ptClient = {clientMousePos.x, clientMousePos.y};
		ClientToScreen(pImpl_->hWindow_, &ptClient);
		return math::vector2i(ptClient.x, ptClient.y);
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/