#ifndef DJAH_VIDEO_DRIVERS_OGL_INDEX_BUFFER_HPP
#define DJAH_VIDEO_DRIVERS_OGL_INDEX_BUFFER_HPP

#include "buffer_base.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	enum E_DRAWING_MODE
	{
		EDM_LINES			= GL_LINES,
		EDM_TRIANGLES		= GL_TRIANGLES,
		EDM_TRIANGLE_STRIP	= GL_TRIANGLE_STRIP
	};

	class index_buffer
		: public buffer_base<GL_ELEMENT_ARRAY_BUFFER>
	{
	public:
		index_buffer(size_t size, E_BUFFER_USAGE usage, E_DRAWING_MODE mode = EDM_TRIANGLES, bool auto_alloc = true)
			: buffer_base<GL_ELEMENT_ARRAY_BUFFER>(size, usage, auto_alloc),
			  mode_(mode) {}

		E_DRAWING_MODE drawingMode() const { return mode_; }

	private:
		E_DRAWING_MODE mode_;
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_INDEX_BUFFER_HPP */