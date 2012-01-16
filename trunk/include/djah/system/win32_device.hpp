#ifndef DJAH_SYSTEM_WINDOWS_WIN32_DEVICE_HPP
#define DJAH_SYSTEM_WINDOWS_WIN32_DEVICE_HPP

#include <string>

#include "../platform.hpp"
#include "device.hpp"

namespace djah { namespace system {

	class win32_device 
		: public device
	{
	public:

		win32_device();
		~win32_device();

		virtual void show();
		virtual bool isWindowActive();
		virtual bool hasWindowFocus();

		virtual void setWindowTitle(const std::string &title);

		virtual void swapBuffers();

	private:

		virtual void createImpl();
		virtual void destroyImpl();
		virtual bool runImpl();

		void createWindow();
		void createContext();

		// Platform specific
		void setupPixelFormat();
		friend LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		HINSTANCE	hInstance_;		// Application handler
		HWND		hWindow_;		// Window handler
		HDC			hDC_;			// Drawing context
		HGLRC		hGLRC_;			// OpenGL context
	};

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_WINDOWS_WIN32_DEVICE_HPP */
