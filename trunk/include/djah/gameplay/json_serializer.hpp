#ifndef DJAH_GAMEPLAY_JSON_SERIALIZER_HPP
#define DJAH_GAMEPLAY_JSON_SERIALIZER_HPP

#include <map>
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "djah/math.hpp"
#include "djah/debug/assertion.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	void json_deserialize(T &val, const rapidjson::Value &node, const std::string &name)
	{
		if( ensure(node.HasMember(name.c_str())) )
		{
			const rapidjson::Value &valueNode = node[name.c_str()];
			val = json_serializer<T>::deserialize(valueNode, name);
		}
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct json_serializer;
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<float>
	{
		static float deserialize(const rapidjson::Value &node, const std::string &name)
		{
			check(node.IsDouble());
			return float(node.GetDouble());
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<double>
	{
		static double deserialize(const rapidjson::Value &node, const std::string &name)
		{
			check(node.IsDouble());
			return node.GetDouble();
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<bool>
	{
		static bool deserialize(const rapidjson::Value &node, const std::string &name)
		{
			check(node.IsBool());
			return node.GetBool();
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<int>
	{
		static int deserialize(const rapidjson::Value &node, const std::string &name)
		{
			check(node.IsInt());
			return node.GetInt();
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<std::string>
	{
		static std::string deserialize(const rapidjson::Value &node, const std::string &name)
		{
			check(node.IsString());
			return std::string(node.GetString());
		}
	};
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	struct json_serializer< math::vector<N,T> >
	{
		static math::vector<N,T> deserialize(const rapidjson::Value &node, const std::string &name)
		{
			check(node.IsArray());
			check(node.Size() == N);

			math::vector<N,T> v;
			for(rapidjson::SizeType i = 0; i < N; ++i)
			{
				v[i] = json_serializer<T>::deserialize(node[i], "");
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
			check(node.IsArray());
			check(node.Size() == 4);

			math::quaternion<T> q;
			for(rapidjson::SizeType i = 0; i < 4; ++i)
			{
				q[i] = json_serializer<T>::deserialize(node[i], "");
			}
			return q;
		}
	};
	//----------------------------------------------------------------------------------------------
	template<typename V>
	struct json_serializer< std::map<std::string,V> >
	{
		static std::map<std::string,V> deserialize(const rapidjson::Value &node, const std::string &name)
		{
			check(node.IsObject());

			std::map<std::string, V> dictionary;
			auto itEnd = node.MemberEnd();
			for(auto it = node.MemberBegin(); it != itEnd; ++it)
			{
				const std::string &key = it->name.GetString();
				dictionary[key] = json_serializer<V>::deserialize(it->value, key);
			}
			return dictionary;
		}
	};
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct json_serializer< std::vector<T> >
	{
		static std::vector<T> deserialize(const rapidjson::Value &node, const std::string &name)
		{
			check(node.IsArray());

			const rapidjson::SizeType vecSize = node.Size();
			std::vector<T> vec;
			vec.reserve(vecSize);
			for(rapidjson::SizeType i = 0; i < vecSize; ++i)
			{
				T val = json_serializer<T>::deserialize(node[i], "");
				vec.push_back(val);
			}
			return vec;
		}
	};
	//----------------------------------------------------------------------------------------------




	/*
	//----------------------------------------------------------------------------------------------
	template<typename AttributeTypes>
	class json_serializer2
	{
	private:
		typedef resources::data_object<AttributeTypes> data_object_t;
		typedef data_object_t::data_object_sptr data_object_sptr;

	private:
		//------------------------------------------------------------------------------------------
		template<typename TL>
		struct attribute_deserializer;
		//------------------------------------------------------------------------------------------
		template<>
		struct attribute_deserializer<utils::nulltype>
		{
			static bool execute(attribute_set_t &attributes, data_object_sptr)
			{
				return attributes.empty();
			}
		};
		//------------------------------------------------------------------------------------------
		template<typename H, typename T>
		struct attribute_deserializer< utils::typelist<H,T> >
		{
			//--------------------------------------------------------------------------------------
			static bool execute(attribute_set_t &attributes, data_object_sptr dobj)
			{
				return attribute_deserializer<T>::execute(attributes, dobj);
			}
			//--------------------------------------------------------------------------------------
		};
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		template<typename TL>
		struct attribute_serializer;
		//------------------------------------------------------------------------------------------
		template<>
		struct attribute_serializer<utils::nulltype>
		{
			static bool execute(filesystem::stream_ptr strm, const data_object_sptr &dobj)
			{
				return true;
			}
		};//------------------------------------------------------------------------------------------
		template<typename H, typename T>
		struct attribute_serializer< utils::typelist<H,T> >
		{
			//--------------------------------------------------------------------------------------
			static bool execute(filesystem::stream_ptr strm, const data_object_sptr &dobj)
			{
				auto attribs = dobj->attributes<H>();
				std::stringstream ss;
				auto itEnd = attribs.end();
				for(auto it = attribs.begin(); it != itEnd; ++it)
				{
					ss << type_name<H>::value() << " " << it->first << " = " << serialize_attribute<H>(it->second.value) << "\n";
				}

				strm->write(ss.str());

				return attribute_serializer<T>::execute(strm, dobj);
			}
			//--------------------------------------------------------------------------------------
		};
		//------------------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------------------
		static bool serialize(filesystem::stream_ptr strm, const data_object_sptr &dobj)
		{
			return attribute_serializer<AttributeTypes>::execute(strm, dobj);
		}
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		static bool deserialize(filesystem::stream &stream, data_object_sptr dobj)
		{
			filesystem::memory_stream memStream(&stream);
			const std::string &stringStream = memStream.toString();

			rapidjson::Document *pDoc = new rapidjson::Document;
			docStack_.push(pDoc);
			pDoc->Parse<0>(stringStream.c_str());

			check(!pDoc->HasParseError());
			auto itEnd = pDoc->MemberEnd();
			for(auto it = pDoc->MemberBegin(); it != itEnd; ++it)
			{
			}

			return true;
		}
		//------------------------------------------------------------------------------------------
	};
	*/

} /*resources*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_JSON_SERIALIZER_HPP */