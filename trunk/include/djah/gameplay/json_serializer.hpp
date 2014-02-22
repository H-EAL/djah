#ifndef DJAH_GAMEPLAY_JSON_SERIALIZER_HPP
#define DJAH_GAMEPLAY_JSON_SERIALIZER_HPP

#include <map>
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "djah/math.hpp"
#include "djah/debug/assertion.hpp"
#include "djah/debug/log.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	template<typename T>
	void json_deserialize(const std::string &name, T &val, const rapidjson::Value &node)
	{
		if( ensure(node.HasMember(name.c_str())) )
		{
			const rapidjson::Value &valueNode = node[name.c_str()];
			if( ensure(json_serializer<T>::is_of_type(valueNode)) )
			{
				json_serializer<T>::deserialize(valueNode, val);
			}
		}
	}
	//----------------------------------------------------------------------------------------------
	template<typename T>
	void json_serialize(const std::string &name, const T &val, rapidjson::Value &node)
	{
		/*
		rapidjson::Value valueNode;
		rapidjson::Value nameNode;
		nameNode.SetString(name.c_str());
		json_serializer<T>::serialize(valueNode, val);
		node.AddMember(nameNode, valueNode);
		*/
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct json_serializer;
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<float>
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			return node.IsDouble();
		}

		static void deserialize(const rapidjson::Value &node, float &val)
		{
			check(node.IsDouble());
			val = float(node.GetDouble());
		}

		static void serialize(rapidjson::Value &node, float val)
		{
			node.SetDouble(double(val));
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<double>
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			return node.IsDouble();
		}

		static void deserialize(const rapidjson::Value &node, double &val)
		{
			check(node.IsDouble());
			val = node.GetDouble();
		}

		static void serialize(rapidjson::Value &node, double val)
		{
			node.SetDouble(val);
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<bool>
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			return node.IsBool();
		}

		static void deserialize(const rapidjson::Value &node, bool &val)
		{
			check(node.IsBool());
			val = node.GetBool();
		}

		static void serialize(rapidjson::Value &node, bool val)
		{
			node.SetBool(val);
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<int>
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			return node.IsInt();
		}

		static void deserialize(const rapidjson::Value &node, int &val)
		{
			check(node.IsInt());
			val = node.GetInt();
		}

		static void serialize(rapidjson::Value &node, int val)
		{
			node.SetInt(val);
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<unsigned int>
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			return node.IsUint();
		}

		static void deserialize(const rapidjson::Value &node, unsigned int &val)
		{
			check(node.IsInt());
			val = node.GetUint();
		}

		static void serialize(rapidjson::Value &node, unsigned int val)
		{
			node.SetUint(val);
		}
	};
	//----------------------------------------------------------------------------------------------
	template<>
	struct json_serializer<std::string>
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			return node.IsString();
		}

		static void deserialize(const rapidjson::Value &node, std::string &val)
		{
			check(node.IsString());
			val = std::string(node.GetString(), node.GetStringLength());
		}

		static void serialize(rapidjson::Value &node, const std::string &val)
		{
			node.SetString(val.c_str());
		}
	};
	//----------------------------------------------------------------------------------------------
	template<int N, typename T>
	struct json_serializer< math::vector<N,T> >
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			bool result = node.IsArray() && node.Size() == N;

			for(rapidjson::SizeType i = 0; i < N && result; ++i)
			{
				result = json_serializer<T>::is_of_type(node[i]);
			}

			return result;
		}

		static void deserialize(const rapidjson::Value &node, math::vector<N,T> &val)
		{
			check(node.IsArray());
			check(node.Size() == N);

			for(rapidjson::SizeType i = 0; i < N; ++i)
			{
				json_serializer<T>::deserialize(node[i], val[i]);
			}
		}

		static void serialize(rapidjson::Value &node, const math::vector<N,T> &val)
		{
		}
	};
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct json_serializer< math::quaternion<T> >
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			bool result = node.IsArray() && node.Size() == 4;

			for(rapidjson::SizeType i = 0; i < 4 && result; ++i)
			{
				result = json_serializer<T>::is_of_type(node[i]);
			}

			return result;
		}

		static void deserialize(const rapidjson::Value &node, math::quaternion<T> &val)
		{
			check(node.IsArray());
			check(node.Size() == 4);

			for(rapidjson::SizeType i = 0; i < 4; ++i)
			{
				json_serializer<T>::deserialize(node[i], val[i]);
			}
		}

		static void serialize(rapidjson::Value &node, const math::quaternion<T> &val)
		{
		}
	};
	//----------------------------------------------------------------------------------------------
	template<int N, int M, typename T>
	struct json_serializer< math::matrix<N,M,T> >
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			bool result = node.IsArray() && node.Size() == N;

			for(rapidjson::SizeType i = 0; i < N && result; ++i)
			{
				result = node.IsArray() && node.Size() == M;

				for(rapidjson::SizeType j = 0; j < M && result; ++j)
				{
					result = json_serializer<T>::is_of_type(node[i][j]);
				}
			}

			return result;
		}

		static void deserialize(const rapidjson::Value &node, math::matrix<N,M,T> &val)
		{
			check(node.IsArray());
			check(node.Size() == N);

			for(rapidjson::SizeType i = 0; i < N; ++i)
			{
				check(node[i].IsArray());
				check(node[i].Size() == M);

				for(rapidjson::SizeType j = 0; j < M; ++j)
				{
					json_serializer<T>::deserialize(node[i][j], val[i][j]);
				}
			}
		}

		static void serialize(rapidjson::Value &node, const math::matrix<N,M,T> &val)
		{
		}
	};
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct json_serializer< std::vector<T> >
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			bool result = node.IsArray();

			const rapidjson::SizeType arraySize = node.Size();
			for(rapidjson::SizeType i = 0; i < arraySize && result; ++i)
			{
				result = json_serializer<T>::is_of_type(node[i]);
			}

			return result;
		}

		static void deserialize(const rapidjson::Value &node, std::vector<T> &val)
		{
			check(node.IsArray());

			const rapidjson::SizeType vecSize = node.Size();
			val.clear();
			val.resize(vecSize);
			for(rapidjson::SizeType i = 0; i < vecSize; ++i)
			{
				json_serializer<T>::deserialize(node[i], val[i]);
			}
		}

		static void serialize(rapidjson::Value &node, const std::vector<T> &val)
		{
		}
	};
	//----------------------------------------------------------------------------------------------
	template<typename T>
	struct json_serializer< std::map<std::string,T> >
	{
		static bool is_of_type(const rapidjson::Value &node)
		{
			bool result = node.IsObject();
			if( result )
			{
				const auto itEnd = node.MemberEnd();
				for(auto it = node.MemberBegin(); it != itEnd && result; ++it)
				{
					result = json_serializer<T>::is_of_type(it->value);
				}
			}
			return result;
		}

		static void deserialize(const rapidjson::Value &node, std::map<std::string,T> &val)
		{
			check(node.IsObject());

			val.clear();

			const auto itEnd = node.MemberEnd();
			for(auto it = node.MemberBegin(); it != itEnd; ++it)
			{
				const std::string &key = it->name.GetString();

				if( val.find(key) != val.end() )
				{
					DJAH_GLOBAL_WARNING()
						<< "Duplicate keys (" << key << ") found in attribute \""
						<< it->name.GetString() << "\""
						<< DJAH_END_LOG();
				}
				// Duplicate keys are overridden
				json_serializer<T>::deserialize(it->value, val[key]);
			}
		}

		static void serialize(rapidjson::Value &node, const std::map<std::string,T> &val)
		{
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

			if( ensure(!pDoc->HasParseError()) )
			{
				const auto itEnd = pDoc->MemberEnd();
				for(auto it = pDoc->MemberBegin(); it != itEnd; ++it)
				{
					if( it->value.IsBool() )
					{
						dobj->add(it->name, it->value.GetBool());
					}
				}
			}

			return true;
		}
		//------------------------------------------------------------------------------------------
	};*/

} /*resources*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_JSON_SERIALIZER_HPP */