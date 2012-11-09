#ifndef DJAH_DEBUG_CORE_LOGGER_HPP
#define DJAH_DEBUG_CORE_LOGGER_HPP

#include <list>
#include <vector>
#include <memory>
#include "../core/singleton.hpp"
#include "record.hpp"

namespace djah { namespace debug {

	class basic_sink;

	class core_logger
		: public utils::singleton<core_logger>
	{
		DJAH_MAKE_SINGLETON(core_logger);
		typedef std::shared_ptr<basic_sink> sink_ptr;

	public:
		void addSink(const sink_ptr &pSink);
		void removeSink(const sink_ptr &pSink);

		basic_record& openRecord(const std::string &channels, eLogSeverity severity, int line, const char *file);
		void log(const basic_record &rec);

		void consume();

	private:
		core_logger();
		~core_logger();

	private:
		typedef std::list<sink_ptr> sink_list_t;
		sink_list_t sinks_;

		typedef std::vector<basic_record> record_list_t;
		record_list_t records_;
	};

} /*debug*/ } /*djah*/

#endif /* DJAH_DEBUG_CORE_LOGGER_HPP */