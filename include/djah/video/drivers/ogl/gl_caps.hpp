#ifndef DJAH_VIDEO_DRIVERS_OGL_GL_CAPS_HPP
#define DJAH_VIDEO_DRIVERS_OGL_GL_CAPS_HPP

#include <vector>
#include <string>

namespace djah { namespace video { namespace drivers { namespace ogl {

	class capabilities
	{
	public:

		static void        init();
		static std::string opengl_version();
		static std::string glsl_version();
		static std::string vendor();
		static std::string renderer();
		static bool	       has_extension(const std::string &extension);

		typedef std::vector<std::string> string_list_t;
		static string_list_t s_extensions_;
	};

} /*ogl*/ } /*drivers*/ } /*video*/ } /*djah*/

#endif /* DJAH_VIDEO_DRIVERS_OGL_GL_CAPS_HPP */