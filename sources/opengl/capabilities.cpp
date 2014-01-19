#include "djah/opengl/capabilities.hpp"
#include <algorithm>

namespace djah { namespace opengl {

	//----------------------------------------------------------------------------------------------
	std::string safe_gl_get_string(GLenum name)
	{
		const GLubyte *buf = glGetString(name);
		return buf ? reinterpret_cast<const char*>(buf) : "";
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	std::string safe_gl_get_string(GLenum name, int i)
	{	
		const GLubyte *buf = glGetStringi ? glGetStringi(name, i) : nullptr;
		return buf ? reinterpret_cast<const char*>(buf) : "";
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void capabilities::init()
	{
		const int nbExtensions = value_of<int>(GL_NUM_EXTENSIONS);
		extensions_.reserve(nbExtensions);
		for(int i = 0; i < nbExtensions; ++i)
		{
			extensions_.push_back( safe_gl_get_string(GL_EXTENSIONS, i) );
		}
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	bool capabilities::has_extension(const std::string &extension)
	{
		return std::find(extensions_.begin(), extensions_.end(), extension) != extensions_.end();
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
