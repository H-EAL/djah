#ifndef DJAH_CORE_ENUM_HPP
#define DJAH_CORE_ENUM_HPP

#include <string>
#include <cstring>
#include <cassert>

//------------------------------------------------------------------------------------------------
#define ENUM(ENUM_NS) djah::enum_t<ENUM_NS::Type, ENUM_NS::Descriptors>
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
#define DJAH_BEGIN_ENUM_DECLARATION(ENUM_NS)                                \
    enum Type                                                               \
    {                                                                       \
        ENUM_NS##_Invalid = -1,

#define DJAH_END_ENUM_DECLARATION(ENUM_NS)                                  \
        ENUM_NS##_Count                                                     \
    };                                                                      \
                                                                            \
    struct Descriptors                                                      \
    {                                                                       \
        static const unsigned int enum_size = ENUM_NS##_Count;              \
        static const djah::enum_descriptor<Type> enum_values[enum_size];    \
    };
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
#define DJAH_BUILD_ENUM(ENUM_NS, ENUM_VAL)                                  \
    ENUM_VAL,
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
#define DJAH_BUILD_ENUM_RUNTIME_LOOK_UP(ENUM_NS, ENUM_VAL)                  \
    {															            \
        #ENUM_NS ##"_"#ENUM_VAL,								            \
        ENUM_VAL,												            \
    },
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
#define DJAH_BEGIN_ENUM_DESCRIPTORS_DEFINITION()                            \
    const djah::enum_descriptor<Type> Descriptors::enum_values[] =          \
    {

#define DJAH_END_ENUM_DESCRIPTORS_DEFINTION()                               \
    }
//------------------------------------------------------------------------------------------------


namespace djah {

    template<typename EnumType>
    struct enum_descriptor
    {
        const char * value_string;
        EnumType     enum_value;
    };

    template<typename EnumType, typename EnumDescriptorType>
    class enum_t
    {
    public:
        enum_t(EnumType _value = EnumType(-1))
            : value_(_value)
        {
        }

        enum_t(const char *valueStr)
            : value_(EnumType(-1))
        {
            set_value_if_valid(valueStr, value_);
        }

        enum_t(const std::string &valueStr)
            : value_(EnumType(-1))
        {
            set_value_if_valid(valueStr.c_str(), value_);
        }

        static bool set_value_if_valid(const char *valueStr, EnumType &val)
        {
            auto itEnd = EnumDescriptorType::enum_values + EnumDescriptorType::enum_size;
            auto it = std::find_if(EnumDescriptorType::enum_values, itEnd, [&](const enum_descriptor<EnumType> &enumDescriptor)
            {
                const bool found = strncmp(valueStr, enumDescriptor.value_string, strlen(enumDescriptor.value_string)) == 0;
                if( found )
                {
                    val = enumDescriptor.enum_value;
                }
                return found;
            });

            return it != itEnd;
        }

        inline const char* toString() const
        {
            static const char * invalid_enum_value = "InvalidEnumValue";

            return ((value_ >= 0) && (value_ < EnumDescriptorType::enum_size))
                ? EnumDescriptorType::enum_values[value_].value_string
                : invalid_enum_value;
        }

        inline enum_t<EnumType, EnumDescriptorType>& operator =(const char *valueStr)
        {
            const bool success = set_value_if_valid(valueStr, value_);
            if( !success )
            {
                value_ = EnumType(-1);
            }
            return (*this);
        }

        inline operator EnumType()
        {
            return value_;
        }

    private:
        EnumType value_;
    };

    //----------------------------------------------------------------------------------------------
    // (In/Out)put (from/to) stream
    //----------------------------------------------------------------------------------------------
    template<typename EnumType, typename EnumDescriptorType>
    inline std::ostream& operator <<(std::ostream &out, enum_t<EnumType,EnumDescriptorType> rhs)
    {
        out << rhs.toString();
        return out;
    }
    //----------------------------------------------------------------------------------------------
    template<typename EnumType, typename EnumDescriptorType>
    inline std::istream& operator >>(std::istream &in, enum_t<EnumType,EnumDescriptorType> rhs)
    {
        std::string valueStr;
        in >> valueStr;
        rhs = valueStr;
        return in;
    }
    //----------------------------------------------------------------------------------------------

} /*djah*/

#endif /* DJAH_CORE_ENUM_HPP */