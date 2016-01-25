#ifndef DJAH_DEBUG_ASSERTION_HPP
#define DJAH_DEBUG_ASSERTION_HPP

#include <string>

//--------------------------------------------------------------------------------------------------
#define DJAH_ASSERT_AUX(Condition, File, Line)\
	{\
		static bool always_ignore = false;\
		if( !always_ignore && !(Condition) )\
		{\
			djah::debug::eAssertResult assertResult = djah::debug::assert_function( File, Line, #Condition );\
			if( assertResult == djah::debug::eAR_Retry )\
			{\
				djah::debug::debugger_break();\
			}\
			else if( assertResult == djah::debug::eAR_AlwaysIgnore )\
			{\
				always_ignore = true;\
			}\
		}\
	}\
//--------------------------------------------------------------------------------------------------
#define DJAH_ASSERT(Condition) DJAH_ASSERT_AUX( (Condition), __FILE__, __LINE__ )
#define check(Condition) DJAH_ASSERT(Condition)
#define ensure(Condition) ((Condition) ? true : djah::debug::assert_and_return_false(djah::debug::assert_function(__FILE__, __LINE__, #Condition)))
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
#define DJAH_ASSERT_MSG_AUX(Condition, Msg, File, Line, ...)\
	{\
		static bool always_ignore = false;\
		if( !always_ignore && !(Condition) )\
		{\
			djah::debug::eAssertResult assertResult = djah::debug::assert_function( File, Line, #Condition, Msg, __VA_ARGS__ );\
			if( assertResult == djah::debug::eAR_Retry )\
			{\
				djah::debug::debugger_break();\
			}\
			else if( assertResult == djah::debug::eAR_AlwaysIgnore )\
			{\
				always_ignore = true;\
			}\
		}\
	}\
//--------------------------------------------------------------------------------------------------
#define DJAH_ASSERT_MSG(Condition, Msg, ...) DJAH_ASSERT_MSG_AUX( Condition, Msg, __FILE__, __LINE__, __VA_ARGS__ )
#define checkf(Condition, Msg, ...) DJAH_ASSERT_MSG( Condition, Msg, __VA_ARGS__ )
#define ensuref(Condition, Msg, ...) ((Condition) ? true : djah::debug::assert_and_return_false(djah::debug::assert_function(__FILE__, __LINE__, #Condition, Msg, __VA_ARGS__)))
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
#define DJAH_STATIC_ASSERT(Condition, Msg) static_assert( (Condition), (Msg) )
//--------------------------------------------------------------------------------------------------


namespace djah { namespace debug {

	enum eAssertResult
	{
		eAR_Abort,
		eAR_Retry,
		eAR_Ignore,
		eAR_AlwaysIgnore
	};

	extern std::string	 stack_trace();
	extern void			 debugger_break();
	extern eAssertResult assert_function(const char *file, int line, const char *expression, const char *description = "", ...);
    extern bool			 assert_and_return_false(eAssertResult assertResult);

} /*debug*/ } /*djah*/

#endif /* DJAH_DEBUG_ASSERTION_HPP */