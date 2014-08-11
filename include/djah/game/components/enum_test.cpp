#include "enum_test.hpp"

namespace EnumTest
{
    #define DJAH_ENUM_PROCESSOR DJAH_BUILD_ENUM_RUNTIME_LOOK_UP
    DJAH_BEGIN_ENUM_DESCRIPTORS_DEFINITION()
        #include "enum_test.enum"
    DJAH_END_ENUM_DESCRIPTORS_DEFINTION();
    #undef DJAH_ENUM_PROCESSOR

} /*EnumTest*/


namespace TextureFilter
{
    #define DJAH_ENUM_PROCESSOR DJAH_BUILD_ENUM_RUNTIME_LOOK_UP
    DJAH_BEGIN_ENUM_DESCRIPTORS_DEFINITION()
        #include "enum_texture_filter.enum"
    DJAH_END_ENUM_DESCRIPTORS_DEFINTION();
    #undef DJAH_ENUM_PROCESSOR

} /*TextureFilter*/