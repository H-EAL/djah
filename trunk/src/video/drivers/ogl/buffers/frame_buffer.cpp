#include "video/drivers/ogl/buffers/frame_buffer.hpp"

#include <GL/glew.h>

#include "log/logger.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	//----------------------------------------------------------------------------------------------
	frame_buffer::frame_buffer()
	{
		aquire();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	frame_buffer::~frame_buffer()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void frame_buffer::aquire()
	{
		glGenFramebuffers(1, &id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void frame_buffer::release()
	{
		glDeleteFramebuffers(1, &id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void frame_buffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void frame_buffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/