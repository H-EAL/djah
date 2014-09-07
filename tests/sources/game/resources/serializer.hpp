#ifndef DJAH_DATA_OBJECT_SERIALIZER_HPP
#define DJAH_DATA_OBJECT_SERIALIZER_HPP

#include <string>
#include "djah/math/vector3.hpp"
#include "djah/math/quaternion.hpp"

template<typename T>
struct type_name
{
	static const char* value() { return "unknown_type"; }
};

#define DEFINE_TYPE_NAME(Type)  template<> struct type_name<Type> { static const char* value() { return # Type; } }
#define DEFINE_TYPE_NAME2(Type, Name) template<> struct type_name<Type> { static const char* value() { return Name; } }
#define PRINT_TYPE(Type) type_name<Type>::value()

DEFINE_TYPE_NAME(int);
DEFINE_TYPE_NAME(float);
DEFINE_TYPE_NAME(bool);
DEFINE_TYPE_NAME(double);
DEFINE_TYPE_NAME2(std::string, "string");
DEFINE_TYPE_NAME2(djah::math::vector3f, "vec3f");
DEFINE_TYPE_NAME2(djah::math::quatf, "quaternion");

#endif /* DJAH_DATA_OBJECT_SERIALIZER_HPP */