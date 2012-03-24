#ifndef DJAH_VIDEO_OGL_INDEX_BUFFER_HPP
#define DJAH_VIDEO_OGL_INDEX_BUFFER_HPP

#include "buffer_base.hpp"

namespace djah { namespace video { namespace ogl {

	enum E_DRAWING_MODE
	{
		EDM_LINES			= GL_LINES,
		EDM_TRIANGLES		= GL_TRIANGLES,
		EDM_TRIANGLE_STRIP	= GL_TRIANGLE_STRIP
	};

	class index_buffer
		: public buffer_base
	{
	public:
		index_buffer(size_t size, E_BUFFER_USAGE usage, E_DRAWING_MODE mode = EDM_TRIANGLES, bool auto_alloc = true)
			: buffer_base(EBT_INDEX_BUFFER, usage, size, auto_alloc),
			  mode_(mode) {}

		E_DRAWING_MODE drawingMode() const { return mode_; }

	private:
		E_DRAWING_MODE mode_;
	};

} /*ogl*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_OGL_INDEX_BUFFER_HPP */
