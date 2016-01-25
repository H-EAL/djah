#include "djah/debug/core_logger.hpp"

#include <ctime>
#include <algorithm>
#include "djah/debug/basic_sink.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/debug/log.hpp"

namespace djah { namespace debug {

	//----------------------------------------------------------------------------------------------
    eLogSeverity basic_record::sCurrentSeverity_ = notification;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	core_logger::core_logger()
    {
        DJAH_LOG_TODO("Launch new thread running consume() function");
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
        DJAH_LOG_TODO("Lock sinks");
        // std::unique_lock lock(sinkLock);
		sinks_.push_back(pSink);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void core_logger::removeSink(const sink_ptr &pSink)
    {
        DJAH_LOG_TODO("Lock sinks");
        // std::unique_lock lock(sinkLock);
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
        DJAH_LOG_TODO("Notify condition variable");
        // cv.notify_all();
		basic_record rec( channels, severity, line, file, stack_trace(), static_cast<unsigned int>(::time(nullptr)) );
		records_.push_back(rec);
		return records_.back();
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void core_logger::log(const basic_record &rec)
    {
        DJAH_LOG_TODO("Lock sinks");
		for( auto upSink : sinks_ )
		{
            if (upSink->passFilter(rec))
			{
                upSink->consume(rec);
			}
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void core_logger::consume()
	{
        DJAH_LOG_TODO("Run this in a separate thread and turn records_ into a concurrent FIFO queue");
        /*
        while( !done )
        {
            // Wait until something is logged in
            while(records.empty())
            {
                cv.wait();
            }

            std::unique_lock lock(m);
            {
                const basic_record &record = records_.top();
                records_.pop();
                log(record);
            }
        }
        */

		for( const auto &rec : records_ )
		{
			log(rec);
		}
		records_.clear();
	}
	//----------------------------------------------------------------------------------------------

} /*debug*/ } /*djah*/