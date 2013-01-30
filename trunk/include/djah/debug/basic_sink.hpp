#ifndef DJAH_DEBUG_BASIC_SINK_HPP
#define DJAH_DEBUG_BASIC_SINK_HPP

#include <string>
#include <fstream>
#include "record.hpp"
#include "log_filter.hpp"

namespace djah { namespace debug {

	class basic_sink
	{
	public:
		basic_sink(const log_filter &_filter = log_filter()) : filter_(_filter) {}
		virtual ~basic_sink() {}

		const log_filter& filter() const { return filter_; }
		log_filter&       filter()       { return filter_; }
		
		bool passFilter(const basic_record &rec) const
		{
			return	rec.severity() >= filter_.minSeverity()
				&&	rec.severity() <= filter_.maxSeverity()
				&&	filter_.hasChannel( rec.channels() );
		}
		virtual void consume(const basic_record &rec) = 0;

	private:
		log_filter filter_;
	};

	typedef std::shared_ptr<basic_sink> sink_ptr;

} /*debug*/ } /*debug*/

#endif /* DJAH_DEBUG_BASIC_SINK_HPP */