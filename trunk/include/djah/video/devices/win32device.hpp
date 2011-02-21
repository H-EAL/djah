#ifndef DJAH_VIDEO_DEVICES_WIN32DEVICE_HPP
#define DJAH_VIDEO_DEVICES_WIN32DEVICE_HPP

#include <string>

#include "../../platform.hpp"
#include "../device_base.hpp"

namespace djah { namespace video {
	
	// Forward declaration
	namespace drivers { namespace detail {
		class opengl_win32;
	} /*detail*/ } /*drivers*/

	namespace devices  {

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
		friend class drivers::detail::opengl_win32;
		friend LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		HINSTANCE	hInstance_;		// Application handler
		HWND		hWindow_;		// Window handler
	};

} /*devices*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DEVICES_WIN32DEVICE_HPP */