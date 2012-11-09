#ifndef DJAH_OPENGL_LOGGER_HPP
#define DJAH_OPENGL_LOGGER_HPP

#include "../debug/log.hpp"

#define DJAH_OPENGL_NOTIFICATION()	DJAH_NOTIFICATION(djah::opengl::logger)
#define DJAH_OPENGL_WARNING()		DJAH_WARNING(djah::opengl::logger)
#define DJAH_OPENGL_ERROR()			DJAH_ERROR(djah::opengl::logger)
#define DJAH_OPENGL_CRITICAL()		DJAH_CRITICAL(djah::opengl::logger)

namespace djah { namespace opengl {

	extern debug::basic_logger logger;

} /*opengl*/ } /*djah*/

#endif /* DJAH_OPENGL_LOGGER_HPP */