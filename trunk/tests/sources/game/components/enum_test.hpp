#ifndef GAME_ENUM_TEST_HPP
#define GAME_ENUM_TEST_HPP

#include "djah/core/enum.hpp"

namespace EnumTest
{
    #define DJAH_ENUM_PROCESSOR DJAH_BUILD_ENUM
    DJAH_BEGIN_ENUM_DECLARATION(EnumTest)
        #include "enum_test.enum"
    DJAH_END_ENUM_DECLARATION(EnumTest)
    #undef DJAH_ENUM_PROCESSOR

} /*EnumTest*/

namespace TextureFilter
{
    #define DJAH_ENUM_PROCESSOR DJAH_BUILD_ENUM
    DJAH_BEGIN_ENUM_DECLARATION(TextureFilter)
        #include "enum_texture_filter.enum"
    DJAH_END_ENUM_DECLARATION(TextureFilter)
    #undef DJAH_ENUM_PROCESSOR

} /*TextureFilter*/

#endif /* GAME_ENUM_TEST_HPP */