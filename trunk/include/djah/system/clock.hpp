#ifndef DJAH_SYSTEM_CLOCK_HPP
#define DJAH_SYSTEM_CLOCK_HPP

#include "../types.hpp"

namespace djah { namespace system { 

	class clock
	{
	public:

		static u64 getCurrentTimeInMs()
		{
			return getCurrentTime() * 1000 / getScaleToSecondsFactor();
		}

		static f64 getCurrentTimeInSec()
		{
			return static_cast<f64>(getCurrentTime()) / getScaleToSecondsFactor();
		}

	private:

		// Platform dependent
		static u64 getCurrentTime();
		static u64 getScaleToSecondsFactor();
	};

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_CLOCK_HPP */