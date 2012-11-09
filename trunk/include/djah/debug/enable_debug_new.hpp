#ifndef DJAH_DEBUG_ENABLE_DEBUG_NEW_HPP
#define DJAH_DEBUG_ENABLE_DEBUG_NEW_HPP

#include "memory_tracker.hpp"

//--------------------------------------------------------------------------------------------------
inline void* operator new(size_t size, const char *file, int line)
{
	void *pv = 0;
	if( !djah::debug::memory_tracker::get().allocateBlock(&pv, size, file, line, false) )
	{
		pv = 0;
	}
	return pv;
}
//--------------------------------------------------------------------------------------------------
inline void* operator new[](size_t size, const char *file, int line)
{
	void *pv = 0;
	if( !djah::debug::memory_tracker::get().allocateBlock(&pv, size, file, line, true) )
	{
		pv = 0;
	}
	return pv;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
inline void operator delete(void *pv)
{
	djah::debug::memory_tracker::get().deallocateBlock(pv, false);
}
//--------------------------------------------------------------------------------------------------
inline void operator delete[](void *pv)
{
	djah::debug::memory_tracker::get().deallocateBlock(pv, true);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
inline void operator delete(void *pv, const char *file, int line)
{
	djah::debug::memory_tracker::get().nextDelete(file, line);
	djah::debug::memory_tracker::get().deallocateBlock(pv, file, line, false);
}
//--------------------------------------------------------------------------------------------------
inline void operator delete[](void *pv, const char *file, int line)
{
	djah::debug::memory_tracker::get().deallocateBlock(pv, file, line, true);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
#define new		new (__FILE__, __LINE__)
//--------------------------------------------------------------------------------------------------
#define delete	djah::debug::memory_tracker::nextDelete(__FILE__, __LINE__), delete
//--------------------------------------------------------------------------------------------------

#endif /* DJAH_DEBUG_ENABLE_DEBUG_NEW_HPP */