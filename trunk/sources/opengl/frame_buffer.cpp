#include "djah/system/gl.hpp"
#include "djah/opengl/frame_buffer.hpp"
#include "djah/opengl/render_buffer.hpp"
#include "djah/opengl/texture.hpp"

#include "djah/opengl/opengl_logger.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	frame_buffer::frame_buffer()
		: colorAttachmentOffset_(0)
		, hasDepthAttachment_(false)
		, hasStencilAttachment_(false)
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
	bool frame_buffer::isValidResource() const
	{
		return glIsFramebuffer(id_) != 0;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void frame_buffer::bindReading() const
	{
		monitor<frame_buffer>::bind(id_);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void frame_buffer::bindWriting() const
	{
		monitor<frame_buffer>::bind(id_);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void frame_buffer::unbindReading()
	{
		monitor<frame_buffer>::bind(0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void frame_buffer::unbindWriting()
	{
		monitor<frame_buffer>::bind(0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	int frame_buffer::attach(const texture &tex)
	{
		monitor<frame_buffer>::assert_if_not_bound(id_);

		const int attachmentPoint = getAttachmentPoint( tex.internalFormat() );
		glFramebufferTexture(GL_DRAW_FRAMEBUFFER, attachmentPoint, tex.id(), 0);
		return attachmentPoint;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	int frame_buffer::attach(const render_buffer &renderBuffer)
	{
		monitor<frame_buffer>::assert_if_not_bound(id_);

		const int attachmentPoint = getAttachmentPoint( renderBuffer.internalFormat() );
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, attachmentPoint, GL_RENDERBUFFER, renderBuffer.id());
		return attachmentPoint;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	int frame_buffer::getAttachmentPoint(int internalFormat)
	{
		int attachmentPoint = -1;

		switch( internalFormat )
		{
		case GL_DEPTH_COMPONENT:
		case GL_DEPTH_COMPONENT32F:
			attachmentPoint = GL_DEPTH_ATTACHMENT;
			hasDepthAttachment_ = true;
			break;

		case GL_DEPTH_STENCIL:
		case GL_DEPTH24_STENCIL8:
		case GL_DEPTH32F_STENCIL8:
			attachmentPoint = GL_DEPTH_STENCIL_ATTACHMENT;
			hasDepthAttachment_   = true;
			hasStencilAttachment_ = true;
			break;

		default:
			attachmentPoint = GL_COLOR_ATTACHMENT0 + colorAttachmentOffset_;
			++colorAttachmentOffset_;
			break;
		}

		return attachmentPoint;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool frame_buffer::isComplete() const
	{
		monitor<frame_buffer>::assert_if_not_bound(id_);
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void frame_buffer::bind_default_frame_buffer()
	{
		unbindWriting();
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
