#ifndef DJAH_SYSTEM_CLOCK_HPP
#define DJAH_SYSTEM_CLOCK_HPP

#include "../types.hpp"

namespace djah { namespace system { 

	class clock
	{
	public:

		template<typename Precision>
		static Precision getCurrentTimeInMs()
		{
			return Precision(getCurrentTime() * 1000) / Precision(getScaleToSecondsFactor());
		}
		
		template<typename Precision>
		static Precision getCurrentTimeInSec()
		{
			return Precision(getCurrentTime()) / Precision(getScaleToSecondsFactor());
		}

	private:

		// Platform dependent
		static u64 getCurrentTime();
		static u64 getScaleToSecondsFactor();
	};

} /*system*/ } /*djah*/

#endif /* DJAH_SYSTEM_CLOCK_HPP */