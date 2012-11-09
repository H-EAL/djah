#ifndef DJAH_DEBUG_LOG_HPP
#define DJAH_DEBUG_LOG_HPP

#include "core_logger.hpp"
#include "basic_logger.hpp"
#include "basic_sink.hpp"
#include "record.hpp"

//------------------------------------------------------------------------------
// Inputs TODO notes in output console
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : TODO: "
#ifdef _MSC_VER	
#define DJAH_LOG_TODO(x) __pragma( message(__LOC__ #x) )
#else
#define DJAH_LOG_TODO(x)  /*_Pragma( message(__LOC__ #x) )*/
#endif
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// GLobal logger - all channels
//------------------------------------------------------------------------------
#define DJAH_LOG_CH_SEV(LG, CHANNEL, SEVERITY) LG.openRecord(CHANNEL, djah::debug:: SEVERITY, __LINE__, __FILE__)
//------------------------------------------------------------------------------
#define DJAH_GLOBAL_LOG_SEV(SEVERITY)	DJAH_LOG_CH_SEV(djah::debug::core_logger::get(), "all", SEVERITY)
//------------------------------------------------------------------------------
#define DJAH_GLOBAL_NOTIFICATION()		DJAH_GLOBAL_LOG_SEV(notification)
#define DJAH_GLOBAL_WARNING()			DJAH_GLOBAL_LOG_SEV(warning)
#define DJAH_GLOBAL_ERROR()				DJAH_GLOBAL_LOG_SEV(error)
#define DJAH_GLOBAL_CRITICAL()			DJAH_GLOBAL_LOG_SEV(critical)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Specific logger - one channel
//------------------------------------------------------------------------------
#define DJAH_LOG_SEV(LG, SEVERITY)		LG.openRecord(djah::debug:: SEVERITY, __LINE__, __FILE__)
//------------------------------------------------------------------------------
#define DJAH_NOTIFICATION(LG)			DJAH_LOG_SEV(LG, notification)
#define DJAH_WARNING(LG)				DJAH_LOG_SEV(LG, warning)
#define DJAH_ERROR(LG)					DJAH_LOG_SEV(LG, error)
#define DJAH_CRITICAL(LG)				DJAH_LOG_SEV(LG, critical)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Retro compatibility
//------------------------------------------------------------------------------
#define DJAH_BEGIN_LOG(SEVERITY)		DJAH_GLOBAL_LOG_SEV(SEVERITY)
//------------------------------------------------------------------------------
#define DJAH_END_LOG()					""; djah::debug::core_logger::get().consume()
//------------------------------------------------------------------------------



#endif /* DJAH_DEBUG_LOG_HPP */