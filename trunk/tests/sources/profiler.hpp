#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <djah/debug/log.hpp>
#include <djah/core/time/timer.hpp>

class ScopedProfile
{
public:
	ScopedProfile(const std::string &sectionName)
		: section_name_(sectionName)
		, clock_(true)
	{
		++i;
	}

	~ScopedProfile()
	{
		clock_.pause();
		--i;
		int j = i;

		std::string spaces;
		while(j-- > 0)
			spaces += "    ";

		DJAH_BEGIN_LOG(notification)
			<< spaces
			<< "[" << section_name_ << "] "
			<< "(" << clock_.getElapsedTimeMs() << " ms)"
			<< DJAH_END_LOG();
	}

private:
	std::string section_name_;
	djah::time::timer clock_;
	static int i;
};

//int ScopedProfile::i = 0;

#define DJAH_AUTO_PROFILE(S) ScopedProfile autoScopedProfile(S);

#endif /* PROFILER_HPP */