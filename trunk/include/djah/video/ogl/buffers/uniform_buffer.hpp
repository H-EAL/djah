#ifndef DJAH_VIDEO_OGL_UNIFORM_BUFFER_HPP
#define DJAH_VIDEO_OGL_UNIFORM_BUFFER_HPP

#include "buffer_base.hpp"

namespace djah { namespace video { namespace ogl {

	class uniform_buffer
		: public buffer_base
	{
	public:
		uniform_buffer(size_t size, E_BUFFER_USAGE usage, bool auto_alloc = true)
			: buffer_base(EBT_UNIFORM_BUFFER, usage, size, auto_alloc) {}
	};

} /*ogl*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_OGL_UNIFORM_BUFFER_HPP */
