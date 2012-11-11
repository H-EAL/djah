#ifndef DJAH_OPENGL_RENDER_BUFFER_HPP
#define DJAH_OPENGL_RENDER_BUFFER_HPP

#include "djah/system/gl.hpp"
#include "resource.hpp"

namespace djah { namespace opengl {

	class render_buffer
		: public resource
	{
		DJAH_OPENGL_RESOURCE(render_buffer);

	public:
		render_buffer(int _internalFormat, int _width, int _height);
		virtual ~render_buffer();

		void bind() const;
		static void unbind();

		int width() const			{ return width_; }
		int height() const			{ return height_; }
		int internalFormat() const	{ return internalFormat_; }

	private:
		void initialize();

	private:
		virtual void aquire();
		virtual void release();
		virtual bool isValidResource() const;

	private:
		int  internalFormat_;
		int  width_;
		int  height_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_RENDER_BUFFER_HPP */
