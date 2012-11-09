#ifndef DJAH_DEBUG_DISABLE_DEBUG_NEW_HPP
#define DJAH_DEBUG_DISABLE_DEBUG_NEW_HPP

//--------------------------------------------------------------------------------------------------
#ifdef new
#	undef new
#endif
//--------------------------------------------------------------------------------------------------
#ifdef delete
#	undef delete
#endif
//--------------------------------------------------------------------------------------------------

#endif /* DJAH_DEBUG_DISABLE_DEBUG_NEW_HPP */