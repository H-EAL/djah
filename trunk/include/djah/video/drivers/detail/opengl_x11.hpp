#ifndef DJAH_VIDEO_DRIVERS_DETAIL_OPENGL_X11_HPP
#define DJAH_VIDEO_DRIVERS_DETAIL_OPENGL_X11_HPP

#include <GL/gl.h>

#include "opengl_impl.hpp"

namespace djah { namespace video {
	
	namespace devices {
		class x11device;
	}

	namespace drivers { namespace detail {

	class opengl_x11: public opengl_impl
	{
	public:

		opengl_x11();
		virtual ~opengl_x11();

		virtual void create(device_ptr device);
		virtual void destroy();

	private:
	};

} /*detail*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_DETAIL_OPENGL_X11_HPP */