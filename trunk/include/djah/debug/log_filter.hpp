#ifndef DJAH_DEBUG_LOG_FILTER_HPP
#define DJAH_DEBUG_LOG_FILTER_HPP

#include <set>
#include <string>
#include "record.hpp"

namespace djah { namespace debug {

	class log_filter
	{
	public:
		log_filter(
			const std::string &channels = "any",
			eLogSeverity _minSeverity = notification,
			eLogSeverity _maxSeverity = critical
		);

		void setSeverityRange(eLogSeverity _minSeverity, eLogSeverity _maxSeverity);
		void addChannel(const std::string &channel);

		eLogSeverity minSeverity() const { return minSeverity_; }
		eLogSeverity maxSeverity() const { return maxSeverity_; }
		bool		 hasChannel(const std::string &channel) const;

	private:
		eLogSeverity		  minSeverity_;
		eLogSeverity		  maxSeverity_;
		std::set<std::string> channels_;
		bool				  anyChannelPasses_;
	};

} /*debug*/ } /*djah*/

#endif /* DJAH_DEBUG_LOG_FILTER_HPP */