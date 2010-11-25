#include <cstdarg>
#include <cassert>
#include "log/logger.hpp"

namespace djah { namespace log {

	//--------------------------------------------------------------------------
	logger::E_WARNING_LEVEL logger::level_ = logger::EWL_MEDIUM;
	logger *logger::instance_ = 0;
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	logger::logger()
	{
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	logger::~logger()
	{
		//if( instance_ )
		//	delete instance_;
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	void logger::setLogger(logger *l)
	{
		if( instance_ )
			delete instance_;
		instance_ = l;
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	void logger::setLevel(E_WARNING_LEVEL level)
	{
		if(level < EWL_COUNT)
			level_ = level;
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	void logger::log(const char *format, ...)
	{
		char buffer[512];
		va_list params;
		va_start(params, format);
		vsprintf_s(buffer, 512, format, params);
		va_end(params);

		instance_->beginLog();
		instance_->write(buffer);
		instance_->endLog();
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	logger& logger::log(E_WARNING_LEVEL level)
	{
		assert(instance_ != 0);
		if(level != EWL_USELAST)
			setLevel(level);
		instance_->beginLog();
		return *instance_;
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	std::string logger::endl()
	{
		assert(instance_ != 0);
		return instance_->endLog();
	}
	//--------------------------------------------------------------------------

} /*log*/ } /*djah*/