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

		void createWindow(device *pThisDevice);
		void destroyWindow();
		bool setupFullScreen(int width, int height);
		bool setupWindowed(int width, int height);
		void setupPixelFormat(int colorBits, int depthBits, int stencilBits);
		bool pumpMessages();

		HINSTANCE	hInstance_;		// Application handler
		HWND		hWindow_;		// Window handler
		HDC			hDC_;			// Device Context handler

		friend LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		static bool windowCreated = false;
		if(msg == WM_CREATE || msg == WM_NCCREATE)
		{
			DJAH_LOG_TODO("Fix this UserData ptr stuff");
			SetWindowLongPtr(hWnd, GWLP_USERDATA,
				LONG_PTR(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams));
			windowCreated = true;
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		device *pThisDevice = reinterpret_cast<device*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		check(!windowCreated || pThisDevice);
		
		switch(msg)
		{	
		case WM_CLOSE:
			pThisDevice->shutDown();
			break;

		case WM_SIZE:
			//pThisDevice->resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_CHAR:
			// -> buffer this
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device_impl::createWindow(device *pThisDevice)
	{
		const device_config_sptr &pConfig = pThisDevice->config();

		hInstance_ = GetModuleHandle(0);
		check(hInstance_);

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
			LPVOID(pThisDevice)
		);
		check(hWindow_);

		hDC_ = GetDC(hWindow_);
		check(hDC_);

		setupPixelFormat(pConfig->colorBits, pConfig->depthBits, pConfig->stencilBits);

		bool resolutionSuccessfullySetup = false;
		if( pConfig->fullscreen )
		{
			resolutionSuccessfullySetup = setupFullScreen(pConfig->width, pConfig->height);
		}
		else
		{
			resolutionSuccessfullySetup = setupWindowed(pConfig->width, pConfig->height);
		}
		check(resolutionSuccessfullySetup);

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
			BYTE(colorBits),
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
			BYTE(depthBits),
			BYTE(stencilBits),
			0,
			0, 0, 0, 0, 0
		};

		const int pixelFormat = ChoosePixelFormat(hDC_, &pfd);
		if (!pixelFormat)
		{
			MessageBoxA(hWindow_, "Mode graphique non supporté", "Problem", MB_ICONERROR | MB_OK);
			exit(1);
		}	
		if (!SetPixelFormat(hDC_, pixelFormat, &pfd))
		{
			MessageBoxA(hWindow_, "Mode graphique non supporté", "Problem", MB_ICONERROR | MB_OK);
			exit(1);
		}	
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device_impl::setupFullScreen(int width, int height)
	{
		DEVMODE devMode;
		devMode.dmSize = sizeof(DEVMODE);

		int i = 0;
		bool displaySettingsFound = false;
		while( !displaySettingsFound && EnumDisplaySettings(nullptr, i++, &devMode) )
		{
			displaySettingsFound = (devMode.dmPelsWidth == width) && (devMode.dmPelsHeight == height);
		}

		bool success = false;

		if( displaySettingsFound )
		{
			const int result = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
			if( result == DISP_CHANGE_SUCCESSFUL )
			{
				SetWindowLongPtr(hWindow_, GWL_STYLE,
					WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
				MoveWindow(hWindow_, 0, 0, width, height, TRUE);
				success = true;
			}
		}

		return success;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device_impl::setupWindowed(int width, int height)
	{
		const int result = ChangeDisplaySettings(nullptr, 0);
		if( result != DISP_CHANGE_SUCCESSFUL )
			return false;

		// Take into account borders
		int real_width  = width  + (GetSystemMetrics(SM_CXFIXEDFRAME) * 2);
		int real_height = height + GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYFIXEDFRAME) * 2);

		// Center on the screen
		int pos_x = (GetSystemMetrics(SM_CXSCREEN) - real_width)  / 2;
		int pos_y = (GetSystemMetrics(SM_CYSCREEN) - real_height) / 2;

		SetWindowLongPtr(hWindow_, GWL_STYLE,
			WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
		MoveWindow(hWindow_, pos_x, pos_y, real_width, real_height, TRUE);

		return true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device_impl::pumpMessages()
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

} /*system*/ } /*djah*/



namespace djah { namespace system {

	//----------------------------------------------------------------------------------------------
	device::device(const device_config_sptr &_pConfig)
		: pImpl_(new device_impl)
		, pConfig_(_pConfig)
		, hasToQuit_(false)
	{
		DJAH_SYSTEM_NOTIFICATION() << "Creating window..." << DJAH_END_LOG();
		pImpl_->createWindow(this);
		sp_device_inst_ = this;
		DJAH_SYSTEM_NOTIFICATION() << "Window successfully created" << DJAH_END_LOG();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	device::~device()
	{
		DJAH_SYSTEM_NOTIFICATION() << "Destroying device..." << DJAH_END_LOG();
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
		return !hasToQuit_ && pImpl_->pumpMessages();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void device::shutDown()
	{
		hasToQuit_ = true;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool device::setResolution(int width, int height, bool fullScreen)
	{
		bool success = false;

		if( fullScreen )
		{
			DJAH_SYSTEM_NOTIFICATION() << "Trying to set fullscreen resolution to " << width << "x" << height << DJAH_END_LOG();
			success = pImpl_->setupFullScreen(width, height);
		}
		else
		{
			DJAH_SYSTEM_NOTIFICATION() << "Trying to set window resolution to " << width << "x" << height << DJAH_END_LOG();
			success = pImpl_->setupWindowed(width, height);
		}

		if( success )
		{
			DJAH_SYSTEM_NOTIFICATION() << "Resolution successfully set to " << width << "x" << height << DJAH_END_LOG();
			pConfig_->width = width;
			pConfig_->height = height;
			pConfig_->fullscreen = fullScreen;
		}
		else
		{
			DJAH_SYSTEM_WARNING()
				<< "Unable to set resolution to "
				<< width << "x" << height
				<< " in " << (fullScreen ? "fullscreen" : "windowed") << " mode"
				<< DJAH_END_LOG();
		}

		return success;
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

} /*system*/ } /*djah*/