#ifndef DJAH_LOG_HTML_LOGGER_HPP
#define DJAH_LOG_HTML_LOGGER_HPP

#include <fstream>
#include "../filesystem/stream.hpp"
#include "logger.hpp"

namespace djah { namespace log {
	
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

} /*log*/ } /*djah*/

#endif /* DJAH_LOG_HTML_LOGGER_HPP */