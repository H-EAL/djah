#include "video/drivers/ogl/buffers/texture.hpp"
#include "video/drivers/opengl_include.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	//----------------------------------------------------------------------------------------------
	texture::texture(int width, int height)
		: width_(width)
		, height_(height)
	{
		aquire();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	texture::~texture()
	{
		release();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::aquire()
	{
		glGenTextures(1, &id_);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void texture::release()
	{
		glDeleteTextures(1, &id_);
		id_ = INVALID_ID;
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/