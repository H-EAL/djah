#ifndef DJAH_TIME_CLOCK_HPP
#define DJAH_TIME_CLOCK_HPP

#include "../types.hpp"

namespace djah { namespace time { 

	class clock
	{
	public:

		clock(bool startNow = true);

		void restart();
		void pause();
		void unpause();

		u64  getElapsedTimeMs() const;
		f32  getElapsedTimeSec() const;

	private:

		u64  getCurrentTime() const;

		u64  start_time_;
		u64  paused_time_;
		u64  dead_time_;
		bool paused_;
	};

} /*time*/ } /*djah*/

#endif /* DJAH_TIME_CLOCK_HPP */