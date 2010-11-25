#ifndef DJAH_VIDEO_DRIVERS_OPENGL_BASE_HPP
#define DJAH_VIDEO_DRIVERS_OPENGL_BASE_HPP

#include "../driver_base.hpp"
#include "platform.hpp"

namespace djah { namespace video { namespace drivers {

	class opengl_base
		: public driver_base
		, public OglImpl
	{
	public:

		opengl_base(device_base *device);
		virtual ~opengl_base();

		virtual void create();
		virtual void destroy();

		virtual void beginScene();
		virtual void endScene();

	protected:

		virtual void updateViewport();
	};

} /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OPENGL_BASE_HPP */