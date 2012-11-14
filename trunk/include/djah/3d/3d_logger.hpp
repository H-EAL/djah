#ifndef DJAH_3D_LOGGER_HPP
#define DJAH_3D_LOGGER_HPP

#include "djah/debug/log.hpp"

#define DJAH_3D_NOTIFICATION()	DJAH_NOTIFICATION(djah::d3d::logger)
#define DJAH_3D_WARNING()		DJAH_WARNING(djah::d3d::logger)
#define DJAH_3D_ERROR()			DJAH_ERROR(djah::d3d::logger)
#define DJAH_3D_CRITICAL()		DJAH_CRITICAL(djah::d3d::logger)

namespace djah { namespace d3d {

	extern debug::basic_logger logger;

} /*d3d*/ } /*djah*/

#endif /* DJAH_3D_LOGGER_HPP */