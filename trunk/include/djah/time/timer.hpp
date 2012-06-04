#ifndef DJAH_TIME_TIMER_HPP
#define DJAH_TIME_TIMER_HPP

#include "../types.hpp"

namespace djah { namespace time { 

	class timer
	{
	public:
		timer(bool startNow = true);

		void restart();
		void pause();
		void unpause();

		f32  getElapsedTimeMs() const;
		f32  getElapsedTimeSec() const;

	private:
		f32  getCurrentTime() const;

		f32  start_time_;
		f32  paused_time_;
		f32  dead_time_;
		bool paused_;
	};

} /*time*/ } /*djah*/

#endif /* DJAH_TIME_TIMER_HPP */