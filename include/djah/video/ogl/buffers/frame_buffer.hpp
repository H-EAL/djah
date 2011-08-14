#ifndef DJAH_VIDEO_OGL_FRAME_BUFFER_HPP
#define DJAH_VIDEO_OGL_FRAME_BUFFER_HPP

#include "../resource.hpp"

namespace djah { namespace video { namespace ogl {

	class texture;
	class render_buffer;

	class frame_buffer
		: public resource
	{
	public:

		frame_buffer();
		virtual ~frame_buffer();

		void bind() const;
		static void unbind();

		void attachTexture(const texture &tex);
		void attachRenderBuffer(const render_buffer &renderBuffer);

	private:

		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;
	};

} /*ogl*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_OGL_FRAME_BUFFER_HPP */
