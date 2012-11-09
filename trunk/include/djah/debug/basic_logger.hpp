#ifndef DJAH_DEBUG_LOGGER_HPP
#define DJAH_DEBUG_LOGGER_HPP

#include "record.hpp"

namespace djah { namespace debug {

	class basic_logger
	{
	public:
		basic_logger(const std::string &channel);
		virtual ~basic_logger();

		basic_record& openRecord(eLogSeverity severity, int line, const char *file);

	private:
		std::string channel_;
	};

} /*debug*/ } /*djah*/

#endif /* DJAH_DEBUG_LOGGER_HPP */