#ifndef DJAH_DEBUG_CONSOLE_SINK_HPP
#define DJAH_DEBUG_CONSOLE_SINK_HPP

#include "basic_sink.hpp"

namespace djah { namespace debug {

	//----------------------------------------------------------------------------------------------
	class console_sink
		: public basic_sink
	{
	public:
		console_sink(const std::string &channels = "all");
		virtual ~console_sink();

		virtual void consume(const basic_record &rec);
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class output_debug_sink
		: public basic_sink
	{
	public:
		output_debug_sink(const std::string &channels = "all");
		virtual ~output_debug_sink();

		virtual void consume(const basic_record &rec);
	};
	//----------------------------------------------------------------------------------------------

} /*debug*/ } /*debug*/

#endif /* DJAH_DEBUG_CONSOLE_SINK_HPP */