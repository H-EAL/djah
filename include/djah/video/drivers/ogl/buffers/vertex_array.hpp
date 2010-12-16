#ifndef DJAH_VIDEO_DRIVERS_OGL_VERTEX_ARRAY_HPP
#define DJAH_VIDEO_DRIVERS_OGL_VERTEX_ARRAY_HPP

#include "../resource.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	// Forward declarations
	class vertex_buffer;
	class index_buffer;
	class vertex_format;
	class shader;

	class vertex_array
		: public resource
	{
	public:

		vertex_array(const vertex_buffer &vb, const index_buffer &ib, const vertex_format &format);
		~vertex_array();

		void bind() const;
		static void unbind();

		void init(const shader &sp) const;
		void draw() const;

	private:

		void aquire();
		void release();

		const vertex_format &vertex_format_;
		const vertex_buffer &vertex_buffer_;
		const index_buffer  &index_buffer_;
		mutable bool        initialized_;
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_VERTEX_ARRAY_HPP */