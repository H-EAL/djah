#ifndef DJAH_DEBUG_ASSERTION_HPP
#define DJAH_DEBUG_ASSERTION_HPP

#include <cassert>

#define DJAH_ASSERT(Exp) assert( (Exp) );

#define DJAH_ASSERT_MSG(Exp, Msg) assert( (Exp) && (Msg) )

#define DJAH_STATIC_ASSERT(Exp, Msg) static_assert( (Exp), (Msg) );

#endif /* DJAH_DEBUG_ASSERTION_HPP */