#include "log/html_logger.hpp"
#include "fs/filesystem.hpp"

namespace djah { namespace log {
	
	//--------------------------------------------------------------------------
	html_logger::html_logger(const std::string &url)
		: log_stream_(fs::filesystem::get_instance().openWriteStream(url))
	{
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
		std::string parsed_msg = msg;

		std::string br = "<br/>";
		size_t n = parsed_msg.find("\n", 0);
		while(n != std::string::npos)
		{
			parsed_msg.erase(n, 1);		// erase /n
			parsed_msg.insert(n, br);	// insert <br/>
			n = parsed_msg.find("\n", n);
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