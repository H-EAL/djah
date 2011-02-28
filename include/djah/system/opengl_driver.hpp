#ifndef DJAH_SYSTEM_OPENGL_DRIVER_HPP
#define DJAH_SYSTEM_OPENGL_DRIVER_HPP

#include "driver_base.hpp"

namespace djah { namespace system {

	class opengl_context;

	class opengl_driver
		: public driver_base
	{
	public:
		opengl_driver(device_base *device);
		virtual ~opengl_driver();

		virtual void create();
		virtual void destroy();
		virtual void swapBuffers();

		opengl_context* context() const;

		virtual void beginScene();
		virtual void endScene();


	protected:
		virtual void updateViewport();

	private:
		opengl_context *context_;
	};

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_OPENGL_DRIVER_HPP */