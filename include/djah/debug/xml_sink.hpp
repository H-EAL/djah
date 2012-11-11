#ifndef DJAH_DEBUG_XML_SINK_HPP
#define DJAH_DEBUG_XML_SINK_HPP

#include <string>
#include <fstream>
#include "basic_sink.hpp"

namespace djah { namespace debug {

	class xml_sink
		: public basic_sink
	{
	public:
		xml_sink(const std::string &filePath, const log_filter &_filter = log_filter());
		virtual ~xml_sink();

		virtual void consume(const basic_record &rec);

	private:
		std::string  filePath_;
		std::fstream fileStream_;
	};

} /*debug*/ } /*debug*/

#endif /* DJAH_DEBUG_XML_SINK_HPP */