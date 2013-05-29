#include "djah/system/device.hpp"
#include "djah/system/video_config.hpp"
#include "djah/system/system_logger.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/platform.hpp"


namespace djah { namespace system {
	
	//----------------------------------------------------------------------------------------------
	class device_impl
	{
	public:

		device_impl() : hInstance_(0), hWindow_(0), hDC_(0) {}
		~device_impl() {}

		void createWindow(const device_config_sptr &pConfig);
		void destroyWindow();
		void setupFullScreen(int width, int height);
		void setupWindowed(int width, int height);

		// Platform specific
		void setupPixelFormat(int colorBits, int depthBits, int stencilBits);
		HINSTANCE	hInstance_;		// Application handler
		HWND		hWindow_;		// Window handler
		HDC			hDC_;			// Device Context handler

		friend LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		device *pDevice = device::get_current();

		check(pDevice);

		switch(msg)
		{	
		case WM_CLOSE:
			pDevice->shutDown();
			break;

		case WM_SIZE:
			pDevice->resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_CREATE:
			break;

		case WM_CHAR:
			// -> buffer this
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device_impl::createWindow(const device_config_sptr &pConfig)
	{
		hInstance_ = GetModuleHandle(0);
		DJAH_ASSERT(hInstance_);

		static int nbClass = 0;
		std::stringstream ss;
		ss << "win32_device_" << (nbClass++);
		const std::string &className = ss.str();

		WNDCLASS windowClass =
		{
			CS_OWNDC | CS_VREDRAW | CS_HREDRAW,
			WinProc,
			0, 0, hInstance_,
			0, LoadCursor(0, IDC_ARROW),
			0, 0, className.c_str()
		};
		check( RegisterClass(&windowClass) );
		
		hWindow_ = CreateWindow
		(
			className.c_str(),
			pConfig->title.c_str(),
			0,
			10, 10,
			10, 10,
			0, 0,
			hInstance_,
			0
		);
		check(hWindow_);

		hDC_ = GetDC(hWindow_);
		check(hDC_);

		setupPixelFormat(pConfig->colorBits, pConfig->depthBits, pConfig->stencilBits);

		if( pConfig->fullscreen )
		{
			setupFullScreen(pConfig->width, pConfig->height);
		}
		else
		{
			setupWindowed(pConfig->width, pConfig->height);
		}

		SetActiveWindow(hWindow_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device_impl::destroyWindow()
	{
		ReleaseDC(hWindow_, hDC_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device_impl::setupPixelFormat(int colorBits, int depthBits, int stencilBits)
	{
		PIXELFORMATDESCRIPTOR pfd =
		{	 
			sizeof(PIXELFORMATDESCRIPTOR), 1,
			PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			static_cast<BYTE>(colorBits),	// Bits de couleur
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
			static_cast<BYTE>(depthBits),	// Bits de profondeur
			static_cast<BYTE>(stencilBits),	// Bits du buffer stencil
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


	//----------------------------------------------------------------------------------------------
	void device_impl::setupFullScreen(int width, int height)
	{
		DEVMODE devMode;
		devMode.dmSize = sizeof(DEVMODE);

		int i = 0;
		bool displaySettingsFound = false;
		while( !displaySettingsFound && EnumDisplaySettings(NULL, i++, &devMode) )
		{
			displaySettingsFound = (devMode.dmPelsWidth == width) && (devMode.dmPelsHeight == height);
		}
		check(displaySettingsFound);

		const int result = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
		check(result == DISP_CHANGE_SUCCESSFUL);

		SetWindowLongPtr(hWindow_, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
		MoveWindow(hWindow_, 0, 0, width, height, TRUE);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device_impl::setupWindowed(int width, int height)
	{
		const int result = ChangeDisplaySettings(nullptr, 0);
		check(result == DISP_CHANGE_SUCCESSFUL);

		// Take into account borders
		int real_width  = width  + (GetSystemMetrics(SM_CXFIXEDFRAME) * 2);
		int real_height = height + GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYFIXEDFRAME) * 2);

		// Center on the screen
		int pos_x = (GetSystemMetrics(SM_CXSCREEN) - real_width)  / 2;
		int pos_y = (GetSystemMetrics(SM_CYSCREEN) - real_height) / 2;

		RECT rect;
		rect.left   = pos_x;
		rect.top    = pos_y;
		rect.right  = real_width;
		rect.bottom = real_height;

		SetWindowLongPtr(hWindow_, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
		AdjustWindowRect(&rect, WS_CAPTION | WS_POPUPWINDOW, FALSE);
		MoveWindow(hWindow_, pos_x, pos_y, real_width, real_height, TRUE);
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/



namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	device::device(const device_config_sptr &_pConfig)
		: pImpl_(new device_impl)
		, pConfig_(_pConfig)
		, hasToQuit_(false)
	{
		sp_device_inst_ = this;
		pImpl_->createWindow(pConfig_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	device::~device()
	{
		pImpl_->destroyWindow();

		if( sp_device_inst_ == this )
			sp_device_inst_ = nullptr;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void* device::internalHandle() const
	{
		return reinterpret_cast<void*>( pImpl_->hWindow_ );
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
		/*
		auto itEnd = linkedDrivers_.end();
		for(auto it = linkedDrivers_.begin(); it != itEnd; ++it)
		{
			if( !it->expired() )
			{
				//(*it)->setViewport(geometry::rect_i(0,0,width,height));
			}
		}*/
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device::toggleFullScreen(bool fullScreen)
	{
		if( fullScreen )
		{
			pImpl_->setupFullScreen(pConfig_->width, pConfig_->height);
		}
		else
		{
			pImpl_->setupWindowed(pConfig_->width, pConfig_->height);
		}
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	void device::show()
	{
		ShowWindow(pImpl_->hWindow_, SW_SHOW);
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	bool device::isActive() const
	{
		return GetActiveWindow() == pImpl_->hWindow_;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device::hasFocus() const
	{
		return GetFocus() == pImpl_->hWindow_;
	}
	//----------------------------------------------------------------------------------------------

		
	//----------------------------------------------------------------------------------------------
	void device::setTitle(const std::string &title)
	{
		SetWindowText(pImpl_->hWindow_, title.c_str());
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