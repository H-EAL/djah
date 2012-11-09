#ifndef DJAH_SYSTEM_LOGGER_HPP
#define DJAH_SYSTEM_LOGGER_HPP

#include "../debug/log.hpp"

#define DJAH_SYSTEM_NOTIFICATION()	DJAH_NOTIFICATION(djah::system::logger)
#define DJAH_SYSTEM_WARNING()		DJAH_WARNING(djah::system::logger)
#define DJAH_SYSTEM_ERROR()			DJAH_ERROR(djah::system::logger)
#define DJAH_SYSTEM_CRITICAL()		DJAH_CRITICAL(djah::system::logger)

namespace djah { namespace system {

	extern debug::basic_logger logger;

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_LOGGER_HPP */