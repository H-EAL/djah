#ifndef DJAH_VIDEO_DRIVERS_OPENGL_DRIVER_HPP
#define DJAH_VIDEO_DRIVERS_OPENGL_DRIVER_HPP

#include "../driver_base.hpp"

namespace djah { namespace video { namespace drivers {

	namespace detail {
		class opengl_context;
	}

	class opengl_driver
		: public driver_base
	{
	public:
		opengl_driver(device_base *device);
		virtual ~opengl_driver();

		virtual void create();
		virtual void destroy();
		virtual void swapBuffers();

		detail::opengl_context* context() const;

		virtual void beginScene();
		virtual void endScene();


	protected:
		virtual void updateViewport();

	private:
		detail::opengl_context *context_;
	};

} /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OPENGL_DRIVER_HPP */