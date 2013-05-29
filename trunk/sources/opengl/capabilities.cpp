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
		if( !glGetStringi )
			return "";

		const GLubyte *buf = glGetStringi(name, i);
		return buf ? reinterpret_cast<const char*>(buf) : "";
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void capabilities::init()
	{
		const int nbExtensions = valueOf<int>(GL_NUM_EXTENSIONS);
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
