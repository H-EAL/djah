#ifndef DJAH_VIDEO_DRIVERS_OGL_FRAME_BUFFER_HPP
#define DJAH_VIDEO_DRIVERS_OGL_FRAME_BUFFER_HPP

#include "../resource.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	class frame_buffer
		: public resource
	{
	public:

		frame_buffer();
		~frame_buffer();

		void bind() const;
		static void unbind();

	private:

		void aquire();
		void release();
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_FRAME_BUFFER_HPP */