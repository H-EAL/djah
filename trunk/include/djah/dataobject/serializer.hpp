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

/*
namespace djah { namespace dataobject {

	class data_object;

	template<typename TL, typename DES>
	struct deserializer;

	template<typename DES>
	struct deserializer<utils::nulltype, DES>
	{
		static void execute(const std::string &data, data_object &dobj)
		{
			std::cout << "[ERROR] End of typelist" << std::endl;
		}
	};

	template<typename H, typename T, typename DES>
	struct deserializer< utils::typelist<H,T>, DES >
	{
		static void execute(const std::string &data, data_object &dobj)
		{
			std::vector<std::string> strList;
			utils::split_string(data, strList, " ");

			if( strList.size() != 3 )
			{
				std::cout << "[WARNING] invalid line : " << data << std::endl;
				return;
			}

			if( !DES::des<H>(data, dobj) )
			{
				return deserializer<T, DES>::execute(data, dobj);
			}
		}
	};


	struct FOO
	{
		template<typename T>
		static bool des(const std::string &data, data_object &dobj)
		{
			std::vector<std::string> strList;
			utils::split_string(data, strList, " ");

			if( strList[0] == type_name<T>::value() )
			{
				std::cout << "DES<" << PRINT_TYPE(T) << "> deserializes : " << data << std::endl;

				T v;
				std::stringstream ss(strList[2]);
				ss >> v;
				attribute<T> attr(strList[1], v);
				dobj.add( attr );
				return true;
			}

			return false;
		}
	};

	template<>
	static bool FOO::des<bool>(const std::string &data, data_object &dobj)
	{
		std::vector<std::string> strList;
		utils::split_string(data, strList, " ");

		if( strList[0] == "bool" )
		{
			std::cout << "DES<bool> deserializes : " << data << std::endl;
			attribute<bool> attr(strList[1], strList[2] == "true" ? true : false );
			dobj.add( attr );
			return true;
		}

		return false;
	}
	*/
//} /*dataobject*/ } /*serializer*/

#endif /* DJAH_DATA_OBJECT_SERIALIZER_HPP */