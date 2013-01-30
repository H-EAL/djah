#ifndef DJAH_GAMEPLAY_JSON_SERIALIZER_HPP
#define DJAH_GAMEPLAY_JSON_SERIALIZER_HPP

#include <string>
#include "rapidjson/document.h"
#include "djah/math.hpp"
#include "djah/debug/assertion.hpp"

namespace djah {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct json_serializer;
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<float>
	{
		static float deserialize(const rapidjson::Value &node, const std::string &name)
		{
			DJAH_ASSERT(node[name.c_str()].IsDouble());
			return float(node[name.c_str()].GetDouble());
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<double>
	{
		static double deserialize(const rapidjson::Value &node, const std::string &name)
		{
			DJAH_ASSERT(node[name.c_str()].IsDouble());
			return node[name.c_str()].GetDouble();
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<bool>
	{
		static bool deserialize(const rapidjson::Value &node, const std::string &name)
		{
			DJAH_ASSERT(node[name.c_str()].IsBool());
			return node[name.c_str()].GetBool();
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<int>
	{
		static int deserialize(const rapidjson::Value &node, const std::string &name)
		{
			DJAH_ASSERT(node[name.c_str()].IsInt());
			return node[name.c_str()].GetInt();
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<std::string>
	{
		static std::string deserialize(const rapidjson::Value &node, const std::string &name)
		{
			DJAH_ASSERT(node[name.c_str()].IsString());
			return std::string(node[name.c_str()].GetString());
		}
	};
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	struct json_serializer< math::vector<N,T> >
	{
		static math::vector<N,T> deserialize(const rapidjson::Value &node, const std::string &name)
		{
			const rapidjson::Value &a = node[name.c_str()];
			DJAH_ASSERT(a.IsArray() && a.Size() == N);

			math::vector<N,T> v;
			for(rapidjson::SizeType i = 0; i < N; ++i)
			{
				v[i] = T(a[i].GetDouble());
			}
			return v;
		}
	};
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct json_serializer< math::quaternion<T> >
	{
		static math::quatf deserialize(const rapidjson::Value &node, const std::string &name)
		{
			const rapidjson::Value &a = node[name.c_str()];
			DJAH_ASSERT(a.IsArray() && a.Size() == 4);

			math::quaternion<T> q;
			for(rapidjson::SizeType i = 0; i < 4; ++i)
			{
				q[i] = T(a[i].GetDouble());
			}
			return q;
		}
	};
	//----------------------------------------------------------------------------------------------

} /*djah*/

#endif /* DJAH_GAMEPLAY_JSON_SERIALIZER_HPP */