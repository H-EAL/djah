#ifndef DJAH_VIDEO_DRIVERS_OGL_FRAME_BUFFER_HPP
#define DJAH_VIDEO_DRIVERS_OGL_FRAME_BUFFER_HPP

#include "../resource.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	class texture;
	class render_buffer;

	class frame_buffer
		: public resource
	{
	public:

		frame_buffer();
		~frame_buffer();

		void bind() const;
		static void unbind();

		void attachTexture(const texture &tex);
		void attachRenderBuffer(const render_buffer &renderBuffer);

	private:

		void aquire();
		void release();
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_FRAME_BUFFER_HPP */