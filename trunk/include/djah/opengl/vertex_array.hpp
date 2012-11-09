#ifndef DJAH_OPENGL_VERTEX_ARRAY_HPP
#define DJAH_OPENGL_VERTEX_ARRAY_HPP

#include "system/gl.hpp"
#include "vertex_format.hpp"
#include "resource.hpp"

namespace djah { namespace opengl {

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
		void draw(int primitiveType = GL_TRIANGLES) const;

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

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_VERTEX_ARRAY_HPP */
