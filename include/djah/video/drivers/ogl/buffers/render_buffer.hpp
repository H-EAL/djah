#ifndef DJAH_VIDEO_DRIVERS_OGL_RENDER_BUFFER_HPP
#define DJAH_VIDEO_DRIVERS_OGL_RENDER_BUFFER_HPP

#include "../../opengl_include.hpp"
#include "texture.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	enum E_INTERNAL_FORMAT
	{
		EIF_RGB				= GL_RGB,
		EIF_RGBA			= GL_RGBA,
		EIF_DEPTH_COMPONENT	= GL_DEPTH_COMPONENT,
		EIF_STENCIL_INDEX	= GL_STENCIL_INDEX
	};

	class render_buffer
		: public texture
	{
	public:

		render_buffer(int width, int height);
		~render_buffer();

		void bind() const;
		static void unbind();

		void initialize(E_INTERNAL_FORMAT internalFormat);

	private:

		void aquire();
		void release();
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_RENDER_BUFFER_HPP */