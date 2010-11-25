#ifndef DJAH_VIDEO_DRIVERS_DETAIL_OPENGL_WIN32_HPP
#define DJAH_VIDEO_DRIVERS_DETAIL_OPENGL_WIN32_HPP

#include <windows.h>

#include "opengl_impl.hpp"

namespace djah { namespace video {
	
	namespace devices {
		class win32device;
	}

	namespace drivers { namespace detail {

	class opengl_win32
		: public opengl_impl
	{
	public:

		opengl_win32();
		virtual ~opengl_win32();

		virtual void create(device_ptr device);
		virtual void destroy();

	private:

		void setupPixelFormat(devices::win32device *device);
		HGLRC hGLRC_;
	};

} /*detail*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_DETAIL_OPENGL_WIN32_HPP */