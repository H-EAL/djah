#include "log/html_logger.hpp"
#include "filesystem/browser.hpp"

namespace djah { namespace log {
	
	//--------------------------------------------------------------------------
	html_logger::html_logger(const std::string &url)
		: log_stream_(filesystem::browser::get().openWriteStream(url))
	{
		DJAH_LOG_TODO(Use std::stream instead of djah::fs::stream);

		write("<html><head><title>djah Log File</title>\
				<link rel=\"stylesheet\" type=\"text/css\" href=\"log_style.css\"/>\
				<script type=\"text/javascript\" src=\"log_script.js\"></script>\
				</head><body><table>");
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	html_logger::~html_logger()
	{
		write("</table></body></html>");
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	void html_logger::beginLog()
	{
		static std::string tr = "<tr class=\"level_";
		static std::string td = "\"><td>";

		*this << tr << level_ << td;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	void html_logger::write(const std::string &msg)
	{
		std::string parsed_msg("");

		std::string br = "<br/>";
		size_t last = 0;
		size_t n = msg.find("\n", last);
		while(n != std::string::npos)
		{
			parsed_msg += msg.substr(last, n) + br;
			last = n;
			n = msg.find("\n", last);
		}

		log_stream_->write(parsed_msg);
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	std::string html_logger::endLog() const
	{
		return "</td></tr>";
	}
	//--------------------------------------------------------------------------

} /*log*/ } /*djah*/