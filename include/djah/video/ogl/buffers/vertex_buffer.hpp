#ifndef DJAH_VIDEO_OGL_VERTEX_BUFFER_HPP
#define DJAH_VIDEO_OGL_VERTEX_BUFFER_HPP

#include "buffer_base.hpp"

namespace djah { namespace video { namespace ogl {

	class vertex_buffer
		: public buffer_base
	{
	public:
		vertex_buffer(size_t size, E_BUFFER_USAGE usage, bool auto_alloc = true)
			: buffer_base(EBT_VERTEX_BUFFER, usage, size, auto_alloc) {}
	};

} /*ogl*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_OGL_VERTEX_BUFFER_HPP */
