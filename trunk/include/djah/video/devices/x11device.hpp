#ifndef DJAH_VIDEO_DEVICES_X11DEVICE_HPP
#define DJAH_VIDEO_DEVICES_X11DEVICE_HPP

#include <string>

#include <X11/Xlib.h>
#include "../device_base.hpp"

namespace djah { namespace video {
	
	// Forward declaration
	namespace drivers { namespace detail {
		class opengl_x11;
	} /*detail*/ } /*drivers*/

	namespace devices  {

	class x11device : public device_base
	{
	public:

		x11device();
		virtual ~x11device();

		virtual void show();
		virtual void resize(int width, int height);
		virtual bool isWindowActive();
		virtual bool hasWindowFocus();
		virtual void shutDown();

		virtual void setWindowTitle(const std::string &title);

	private:

		virtual void createImpl();
		virtual bool runImpl();

		// Platform specific
		friend class drivers::detail::opengl_x11;
	
		Window 	window_;
		Display *display_;
		
	};


} /*devices*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DEVICES_X11DEVICE_HPP */