#include "djah/opengl/capabilities.hpp"
#include <algorithm>
#include "djah/system/gl.hpp"
#include "djah/debug/assertion.hpp"

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
		const GLubyte *buf = glGetStringi(name, i);
		return buf ? reinterpret_cast<const char*>(buf) : "";
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	capabilities::string_list_t capabilities::sExtensions_;
	capabilities::attrib_map_t  capabilities::sAttributes_;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void capabilities::init()
	{
		const int nbExtensions = value_of(GL_NUM_EXTENSIONS);
		sExtensions_.reserve(nbExtensions);
		for(int i = 0; i < nbExtensions; ++i)
		{
			sExtensions_.push_back( safe_gl_get_string(GL_EXTENSIONS, i) );
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	int capabilities::value_of(int glEnum)
	{
		auto it = sAttributes_.find(glEnum);
		if( it == sAttributes_.end() )
		{
			int val = 0;
			glGetIntegerv(glEnum, &val);
			it = sAttributes_.insert( attrib_map_t::value_type(glEnum, val) ).first;
		}

		DJAH_ASSERT( it != sAttributes_.end() );

		return it->second;
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
		return std::find(sExtensions_.begin(), sExtensions_.end(), extension) != sExtensions_.end();
	}
	//----------------------------------------------------------------------------------------------

} /*opengl*/ } /*djah*/
