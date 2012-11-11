#include "djah/debug/core_logger.hpp"

#include <ctime>
#include <algorithm>
#include "djah/debug/basic_sink.hpp"
#include "djah/debug/assertion.hpp"

namespace djah { namespace debug {

	//----------------------------------------------------------------------------------------------
	eLogSeverity basic_record::currentSeverity = notification;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	core_logger::core_logger()
	{
		records_.reserve(20);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	core_logger::~core_logger()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void core_logger::addSink(const sink_ptr &pSink)
	{
		sinks_.push_back(pSink);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void core_logger::removeSink(const sink_ptr &pSink)
	{
		auto it = std::find(sinks_.begin(), sinks_.end(), pSink);
		if( it != sinks_.end() )
		{
			sinks_.erase(it);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	basic_record& core_logger::openRecord(const std::string &channels, eLogSeverity severity, int line, const char *file)
	{
		basic_record rec( channels, severity, line, file, stack_trace(), static_cast<unsigned int>(::time(nullptr)) );
		records_.push_back(rec);
		return records_.back();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void core_logger::log(const basic_record &rec)
	{
		auto itEnd = sinks_.end();
		for( auto it = sinks_.begin(); it != itEnd; ++it )
		{
			if( (*it)->passFilter(rec) )
			{
				(*it)->consume(rec);
			}
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void core_logger::consume()
	{
		auto itEnd = records_.end();
		for( auto it = records_.begin(); it != itEnd; ++it )
		{
			log(*it);
		}
		records_.clear();
	}
	//----------------------------------------------------------------------------------------------

} /*debug*/ } /*djah*/