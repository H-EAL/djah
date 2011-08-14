#ifndef DJAH_VIDEO_OGL_VERTEX_ARRAY_HPP
#define DJAH_VIDEO_OGL_VERTEX_ARRAY_HPP

#include "vertex_format.hpp"
#include "../resource.hpp"

namespace djah { namespace video { namespace ogl {

	// Forward declarations
	class vertex_buffer;
	class index_buffer;
	class shader_program;

	class vertex_array
		: public resource
	{
	public:

		vertex_array(const vertex_format &format, vertex_buffer *vb, index_buffer *ib = 0);
		virtual ~vertex_array();

		void init(const shader_program &sp) const;
		void draw() const;

	private:

		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

		void bind() const;
		static void unbind();

		vertex_format vertex_format_;
		vertex_buffer *vertex_buffer_;
		index_buffer  *index_buffer_;
		mutable bool  initialized_;
	};

} /*ogl*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_OGL_VERTEX_ARRAY_HPP */
