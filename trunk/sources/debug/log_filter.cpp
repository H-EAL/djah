#include "djah/debug/log_filter.hpp"
#include "djah/core/string_utils.hpp"

namespace djah { namespace debug {

	//----------------------------------------------------------------------------------------------
	log_filter::log_filter( const std::string &channels, eLogSeverity _minSeverity, eLogSeverity _maxSeverity )
		: minSeverity_(_minSeverity)
		, maxSeverity_(_maxSeverity)
		, anyChannelPasses_(false)
	{
		utils::string_list_t splitChannels;
		utils::split_string(channels, splitChannels, ";|,");
		channels_.insert(splitChannels.begin(), splitChannels.end());
		anyChannelPasses_ = (channels_.find("any") != channels_.end());
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void log_filter::setSeverityRange(eLogSeverity _minSeverity, eLogSeverity _maxSeverity)
	{
		minSeverity_ = _minSeverity;
		maxSeverity_ = _maxSeverity;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void log_filter::addChannel(const std::string &channel)
	{
		anyChannelPasses_ |= (channel == "any");
		channels_.insert(channel);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	bool log_filter::hasChannel(const std::string &channel) const
	{
		return anyChannelPasses_ || (channels_.find(channel) != channels_.end());
	}
	//----------------------------------------------------------------------------------------------

} /*debug*/ } /*djah*/