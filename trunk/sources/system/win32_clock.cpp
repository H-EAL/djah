#include "system/clock.hpp"
#include "platform.hpp"

namespace djah { namespace system {
	
	//----------------------------------------------------------------------------------------------
	u64 clock::getCurrentTime()
	{
		LARGE_INTEGER perfCount;
		QueryPerformanceCounter(&perfCount);
		return static_cast<u64>(perfCount.QuadPart);
	}
	//----------------------------------------------------------------------------------------------

	
	//----------------------------------------------------------------------------------------------
	u64 clock::getScaleToSecondsFactor()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return static_cast<u64>(frequency.QuadPart);
	}
	//----------------------------------------------------------------------------------------------

} /*system*/ } /*djah*/