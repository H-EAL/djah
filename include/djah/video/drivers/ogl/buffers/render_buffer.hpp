#ifndef DJAH_VIDEO_DRIVERS_OGL_RENDER_BUFFER_HPP
#define DJAH_VIDEO_DRIVERS_OGL_RENDER_BUFFER_HPP

#include "../resource.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	class render_buffer
		: public resource
	{
	public:

		render_buffer();
		~render_buffer();

		void bind() const;
		static void unbind();

		void initStorage(int width, int height);

	private:

		void aquire();
		void release();
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_RENDER_BUFFER_HPP */