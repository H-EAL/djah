#include "platform.hpp"
#ifdef DJAH_COMPILE_WINDOWS

#include "video/devices/win32device.hpp"

#include <cassert>
#include <GL/gl.h>

#include "geometry/rect.hpp"


namespace djah { namespace video { namespace devices {

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

		case WM_CREATE:
			device->hWindow_ = hWnd;
			device->driver_->create();
			break;

		case WM_SIZE:
			device->resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_KEYUP:
			if(wParam == VK_ESCAPE)
				device->shutDown();
			break;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}
	//----------------------------------------------------------------------------------------------




	//----------------------------------------------------------------------------------------------
	win32device::win32device()
		: hInstance_(0)
		, hWindow_(0)
		, hDC_(0)
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
		int real_width  = video_config_.width_  + (GetSystemMetrics(SM_CXSIZEFRAME) << 1);
		int real_height = video_config_.height_ + GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYSIZEFRAME) << 1);

		// Center on the screen
		int pos_x = (GetSystemMetrics(SM_CXSCREEN) - real_width)  >> 1;
		int pos_y = (GetSystemMetrics(SM_CYSCREEN) - real_height) >> 1;

		long style = WS_VISIBLE;
		long ex_style = 0;
		
		if(video_config_.fullscreen_)
		{
			style |= WS_POPUPWINDOW | WS_MAXIMIZE;
			pos_x = pos_y = 0;
			real_width  = video_config_.width_;
			real_height = video_config_.height_;
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

		UpdateWindow(hWindow_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void win32device::destroyImpl()
	{
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
	void win32device::swapBuffers()
	{
		SwapBuffers(hDC_);
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

} /*devices*/ } /*video*/ } /*djah*/

#endif /*DJAH_COMPILE_WINDOWS */