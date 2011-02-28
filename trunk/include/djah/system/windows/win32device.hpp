#ifndef DJAH_SYSTEM_WINDOWS_WIN32DEVICE_HPP
#define DJAH_SYSTEM_WINDOWS_WIN32DEVICE_HPP

#include <string>

#include "../../platform.hpp"
#include "../device_base.hpp"

namespace djah { namespace system {
	
	class opengl_win32;

	class win32device 
		: public device_base
	{
	public:

		win32device();
		~win32device();

		virtual void show();
		virtual bool isWindowActive();
		virtual bool hasWindowFocus();

		virtual void setWindowTitle(const std::string &title);

	private:

		virtual void createImpl();
		virtual void destroyImpl();
		virtual bool runImpl();

		// Platform specific
		friend class opengl_win32;
		friend LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		HINSTANCE	hInstance_;		// Application handler
		HWND		hWindow_;		// Window handler
	};

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_WINDOWS_WIN32DEVICE_HPP */
