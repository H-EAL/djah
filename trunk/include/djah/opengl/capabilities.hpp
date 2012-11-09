#ifndef DJAH_OPENGL_CAPABILITIES_HPP
#define DJAH_OPENGL_CAPABILITIES_HPP

#include <map>
#include <vector>
#include <string>
#include "core/string_utils.hpp"

namespace djah { namespace opengl {

	class capabilities
	{
	public:

		static void        init();
		static int		   value_of(int glEnum);
		static std::string opengl_version();
		static std::string glsl_version();
		static std::string vendor();
		static std::string renderer();
		static bool	       has_extension(const std::string &extension);

		typedef std::vector<std::string> string_list_t;
		typedef std::map<int, int>		 attrib_map_t;

		static string_list_t sExtensions_;
		static attrib_map_t  sAttributes_;
	};

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_CAPABILITIES_HPP */
