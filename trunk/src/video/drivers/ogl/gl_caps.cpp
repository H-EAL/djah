#include "video/drivers/ogl/gl_caps.hpp"
#include <algorithm>
#include "utils/string_utils.hpp"
#include "video/drivers/opengl_include.hpp"

namespace djah { namespace video { namespace drivers { namespace ogl {

	//----------------------------------------------------------------------------------------------
	std::string safe_gl_get_string(GLenum name)
	{
		const GLubyte *buf = glGetString(name);
		return buf ? reinterpret_cast<const char*>(buf) : "";
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	capabilities::string_list_t capabilities::s_extensions_;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void capabilities::init()
	{
		std::string extensions = safe_gl_get_string(GL_EXTENSIONS);
		utils::split_string(extensions, s_extensions_, " ");
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::string capabilities::vendor()
	{
		return safe_gl_get_string(GL_VENDOR);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::string capabilities::renderer()
	{
		return safe_gl_get_string(GL_RENDERER);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::string capabilities::opengl_version()
	{
		return safe_gl_get_string(GL_VERSION);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	std::string capabilities::glsl_version()
	{
		return safe_gl_get_string(GL_SHADING_LANGUAGE_VERSION);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool capabilities::has_extension(const std::string &extension)
	{
		return std::find(s_extensions_.begin(), s_extensions_.end(), extension) != s_extensions_.end();
	}
	//----------------------------------------------------------------------------------------------

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/