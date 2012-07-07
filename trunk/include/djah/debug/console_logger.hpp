#ifndef DJAH_DEBUG_CONSOLE_LOGGER_HPP
#define DJAH_DEBUG_CONSOLE_LOGGER_HPP

#include "logger.hpp"

namespace djah { namespace debug {
	
	class console_logger
		: public logger_impl
	{
	private:

		void setConsoleColor(bool resetColor = false) const;
		void beginLog();
		void write(const std::string &msg);
		std::string endLog() const;
	};

} /*debug*/ } /*djah*/

#endif /* DJAH_DEBUG_CONSOLE_LOGGER_HPP */