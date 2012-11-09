#include "debug/basic_logger.hpp"

#include <ctime>
#include "debug/record.hpp"
#include "debug/core_logger.hpp"

namespace djah { namespace debug {

	//----------------------------------------------------------------------------------------------
	basic_logger::basic_logger(const std::string &channel)
		: channel_(channel)
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	basic_logger::~basic_logger()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	basic_record& basic_logger::openRecord(eLogSeverity severity, int line, const char *file)
	{
		return core_logger::get().openRecord( channel_, severity, line, file );
	}
	//----------------------------------------------------------------------------------------------

} /*debug*/ } /*djah*/