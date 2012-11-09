#ifndef DJAH_OPENGL_VERTEX_BUFFER_HPP
#define DJAH_OPENGL_VERTEX_BUFFER_HPP

#include "buffer_base.hpp"

namespace djah { namespace opengl {

	class vertex_buffer
		: public buffer_base
	{
	public:
		vertex_buffer(size_t size, eBufferUsage usage, bool auto_alloc = true)
			: buffer_base(eBT_VertexBuffer, usage, size, auto_alloc) {}
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_VERTEX_BUFFER_HPP */
