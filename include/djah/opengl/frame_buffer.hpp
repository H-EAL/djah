#ifndef DJAH_OPENGL_FRAME_BUFFER_HPP
#define DJAH_OPENGL_FRAME_BUFFER_HPP

#include <vector>
#include "resource.hpp"

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	class texture;
	class render_buffer;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class frame_buffer
		: public resource
	{
		DJAH_OPENGL_RESOURCE(frame_buffer);

	public:
		frame_buffer();
		virtual ~frame_buffer();

		void bindReading() const;
		void bindWriting() const;
		static void unbindReading();
		static void unbindWriting();

		int attach(const texture &tex);
		int attach(const render_buffer &renderBuffer);

		bool isComplete() const;

		static void bind_default_frame_buffer();

	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

		int getAttachmentPoint(int internalFormat);

	private:
		int  colorAttachmentOffset_;
		bool hasDepthAttachment_;
		bool hasStencilAttachment_;
	};
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_FRAME_BUFFER_HPP */
