#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <djah/log/logger.hpp>
#include <djah/time/timer.hpp>

class ScopedProfile
{
public:
	ScopedProfile(const std::string &sectionName)
		: section_name_(sectionName)
		, clock_(true)
	{
	}

	~ScopedProfile()
	{
		clock_.pause();
		DJAH_BEGIN_LOG(EWL_NOTIFICATION)
			<< "[" << section_name_ << "] "
			<< "(" << clock_.getElapsedTimeMs() << " ms)"
			<< DJAH_END_LOG();
	}

private:
	std::string section_name_;
	time::timer clock_;
};

#define DJAH_AUTO_PROFILE(S) ScopedProfile autoScopedProfile(S);

#endif /* PROFILER_HPP */