#ifndef DJAH_DEBUG_HTML_LOGGER_HPP
#define DJAH_DEBUG_HTML_LOGGER_HPP

#include <fstream>
#include "../filesystem/stream.hpp"
#include "logger.hpp"

namespace djah { namespace debug {
	
	class html_logger
		: public logger_impl
	{
	public:

		html_logger(const std::string &url);
		~html_logger();

	private:

		void beginLog();
		void write(const std::string &msg);
		std::string endLog() const;

		filesystem::stream_ptr log_stream_;
	};

} /*debug*/ } /*djah*/

#endif /* DJAH_DEBUG_HTML_LOGGER_HPP */