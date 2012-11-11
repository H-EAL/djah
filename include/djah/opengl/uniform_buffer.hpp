#ifndef DJAH_OPENGL_UNIFORM_BUFFER_HPP
#define DJAH_OPENGL_UNIFORM_BUFFER_HPP

#include "buffer_base.hpp"

namespace djah { namespace opengl {

	class uniform_buffer
		: public buffer_base
	{
		DJAH_OPENGL_RESOURCE(uniform_buffer);

	public:
		uniform_buffer(size_t size, eBufferUsage usage, bool auto_alloc = true)
			: buffer_base(eBT_UniformBuffer, usage, size, auto_alloc) {}
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_UNIFORM_BUFFER_HPP */
