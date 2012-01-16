#ifndef DJAH_SYSTEM_OPENGL_CAPABILITIES_HPP
#define DJAH_SYSTEM_OPENGL_CAPABILITIES_HPP

#include <vector>
#include <string>

namespace djah { namespace system { namespace opengl {

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

} /*opengl*/ } /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_OPENGL_CAPABILITIES_HPP */
