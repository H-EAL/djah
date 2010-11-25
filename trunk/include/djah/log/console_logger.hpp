#ifndef DJAH_LOG_CONSOLE_LOGGER_HPP
#define DJAH_LOG_CONSOLE_LOGGER_HPP

#include "logger.hpp"

namespace djah { namespace log {
	
	class console_logger
		: public logger
	{
	private:

		void setConsoleColor(bool resetColor = false) const;
		void beginLog();
		void write(const std::string &msg);
		std::string endLog() const;
	};

} /*log*/ } /*djah*/

#endif /* DJAH_LOG_CONSOLE_LOGGER_HPP */