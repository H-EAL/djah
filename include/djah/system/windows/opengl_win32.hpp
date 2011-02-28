#ifndef DJAH_SYSTEM_OPENGL_WIN32_HPP
#define DJAH_SYSTEM_OPENGL_WIN32_HPP

#include "../../platform.hpp"
#include "../opengl_context.hpp"

namespace djah { namespace system {

	class opengl_win32
		: public opengl_context
	{
	public:

		opengl_win32();
		virtual ~opengl_win32();

		virtual void create(device_ptr device);
		virtual void destroy();
		virtual void swapBuffers();

		virtual void makeCurrent();
		virtual void doneCurrent();

	private:

		void setupPixelFormat(const video_config &cfg);

		HDC hDC_;
		HGLRC hGLRC_;
		HWND hWindow_;
	};

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_OPENGL_WIN32_HPP */
