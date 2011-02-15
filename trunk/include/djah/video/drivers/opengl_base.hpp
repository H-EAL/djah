#ifndef DJAH_VIDEO_DRIVERS_OPENGL_BASE_HPP
#define DJAH_VIDEO_DRIVERS_OPENGL_BASE_HPP

#include "../driver_base.hpp"
#include "video/drivers/ogl/gl_caps.hpp"

namespace djah { namespace video { namespace drivers {

	template<typename OglImpl>
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

#include "opengl_base.inl"

#endif /* DJAH_VIDEO_DRIVERS_OPENGL_BASE_HPP */