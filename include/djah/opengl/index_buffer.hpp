#ifndef DJAH_OPENGL_INDEX_BUFFER_HPP
#define DJAH_OPENGL_INDEX_BUFFER_HPP

#include "buffer_base.hpp"

namespace djah { namespace opengl {

	enum E_DRAWING_MODE
	{
		EDM_LINES			= GL_LINES,
		EDM_TRIANGLES		= GL_TRIANGLES,
		EDM_TRIANGLE_STRIP	= GL_TRIANGLE_STRIP
	};

	class index_buffer
		: public buffer_base
	{
		DJAH_OPENGL_RESOURCE(index_buffer);

	public:
		index_buffer(size_t size, eBufferUsage usage, E_DRAWING_MODE mode = EDM_TRIANGLES, bool auto_alloc = true)
			: buffer_base(eBT_IndexBuffer, usage, size, auto_alloc),
			  mode_(mode) {}

		E_DRAWING_MODE drawingMode() const { return mode_; }

	private:
		E_DRAWING_MODE mode_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_INDEX_BUFFER_HPP */
