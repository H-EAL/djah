#include <cstdarg>
#include <cassert>
#include <cstdio>
#include "log/logger.hpp"

namespace djah { namespace log {

	//--------------------------------------------------------------------------
	E_WARNING_LEVEL logger_impl::level_ = EWL_MEDIUM;
	logger_impl *logger::instance_ = new null_logger;
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	logger::logger()
	{
	}
	//--------------------------------------------------------------------------


	//--------------------------------------------------------------------------
	logger::~logger()
	{
		if( instance_ )
			delete instance_;
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	void logger::setLogger(logger_impl *l)
	{
		if( instance_ )
			delete instance_;
		instance_ = l;
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	void logger::setLevel(E_WARNING_LEVEL level)
	{
		assert(instance_ != 0);
		if(level < EWL_COUNT)
			instance_->level_ = level;
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	void logger::log(const char *format, ...)
	{
		assert(instance_ != 0);

		char buffer[512];
		va_list params;
		va_start(params, format);
		vsprintf(buffer, format, params);
		va_end(params);

		instance_->beginLog();
		instance_->write(buffer);
		instance_->endLog();
	}
	//--------------------------------------------------------------------------
	
	
	//--------------------------------------------------------------------------
	logger_impl& logger::log(E_WARNING_LEVEL level)
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