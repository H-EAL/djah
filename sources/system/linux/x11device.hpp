#ifndef DJAH_SYSTEM_LINUX_X11DEVICE_HPP
#define DJAH_SYSTEM_LINUX_X11DEVICE_HPP

#include <string>

#include <X11/Xlib.h>
#include "../device_base.hpp"
#include "../opengl_include.hpp"

namespace djah { namespace system {

	class x11device
		: public device_base
	{
	public:

		x11device();
		virtual ~x11device();

		virtual void show();
		virtual bool isWindowActive();
		virtual bool hasWindowFocus();

		virtual void setWindowTitle(const std::string &title);

		virtual void swapBuffers();
		
	private:

		virtual void createImpl();
		virtual void destroyImpl();
		virtual bool runImpl();

		// Platform specific	
		Window 	window_;
                Display *display_;
                GLXContext glx_context_;
	};

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_LINUX_X11DEVICE_HPP */