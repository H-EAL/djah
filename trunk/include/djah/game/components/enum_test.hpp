#ifndef DJAH_GAME_ENUM_TEST_HPP
#define DJAH_GAME_ENUM_TEST_HPP

#include "djah/core/enum.hpp"

namespace EnumTest
{
    #define DJAH_ENUM_PROCESSOR DJAH_BUILD_ENUM
    enum Type
    {
        EnumTest_Invalid = -1,
        #include "enum_test.enum"
        EnumTest_Count
    };
    #undef DJAH_ENUM_PROCESSOR

    DJAH_DECLARE_ENUM_DESCRIPTORS(EnumTest);

} /*EnumTest*/

namespace TextureFilter
{
    #define DJAH_ENUM_PROCESSOR DJAH_BUILD_ENUM
    enum Type
    {
        TextureFilter_Invalid = -1,
        #include "enum_texture_filter.enum"
        TextureFilter_Count
    };
    #undef DJAH_ENUM_PROCESSOR

    DJAH_DECLARE_ENUM_DESCRIPTORS(TextureFilter);

} /*TextureFilter*/

#endif /* DJAH_GAME_ENUM_TEST_HPP */