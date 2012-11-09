#ifndef DJAH_DEBUG_BASIC_SINK_HPP
#define DJAH_DEBUG_BASIC_SINK_HPP

#include <string>
#include <fstream>
#include "record.hpp"

namespace djah { namespace debug {

	class basic_sink
	{
	public:
		basic_sink(const std::string &channels = "all") : channels_(channels) {}
		virtual ~basic_sink() {}
		
		bool passFilter(const basic_record &rec) const
		{
			return (channels_ == "all") || (channels_.find(rec.channels()) != std::string::npos);
		}
		virtual void consume(const basic_record &rec) = 0;

	private:
		std::string channels_;
	};

} /*debug*/ } /*debug*/

#endif /* DJAH_DEBUG_BASIC_SINK_HPP */