#ifndef DJAH_VIDEO_DRIVERS_DETAIL_OPENGL_IMPL_HPP
#define DJAH_VIDEO_DRIVERS_DETAIL_OPENGL_IMPL_HPP

#include "../../device_base.hpp"

namespace djah { namespace video { namespace drivers { namespace detail {

	class opengl_impl
	{
	public:

		opengl_impl() {}
		virtual ~opengl_impl() {}

		virtual void create(device_ptr device) = 0;
		virtual void destroy() = 0;
	};

} /*detail*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_DETAIL_OPENGL_IMPL_HPP */