#ifndef DJAH_DATA_OBJECT_INI_SERIALIZER_HPP
#define DJAH_DATA_OBJECT_INI_SERIALIZER_HPP

#include <map>
#include <memory>
#include "djah/core/string_utils.hpp"
#include "djah/filesystem/stream.hpp"
#include "djah/filesystem/memory_stream.hpp"
#include "djah/resources/serializer.hpp"
#include "djah/resources/data_object.hpp"
#include "djah/gameplay/json_serializer.hpp"

namespace djah { namespace resources {

	template<typename AttributeTypes>
	class ini_serializer
	{
	private:
		typedef resources::data_object<AttributeTypes> data_object_t;
		typedef std::shared_ptr<data_object_t> data_object_sptr;

	private:
		//------------------------------------------------------------------------------------------
		typedef std::pair<std::string, std::string>		key_value_t;
		typedef std::vector<key_value_t>				key_value_list_t;
		typedef std::map<std::string, key_value_list_t>	attribute_set_t;
		//------------------------------------------------------------------------------------------


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

			static bool execute(const std::string &attributeName, const rapidjson::Value &attributeValue, data_object_sptr)
			{
				return false;
			}
		};
		//------------------------------------------------------------------------------------------
		template<typename H, typename T>
		struct attribute_deserializer< utils::typelist<H,T> >
		{
			//--------------------------------------------------------------------------------------
			static bool execute(attribute_set_t &attributes, data_object_sptr dobj)
			{
				attribute_set_t::const_iterator itKvList = attributes.find( type_name<H>::value() );
				if( itKvList != attributes.end() )
				{
					key_value_list_t::const_iterator it;
					key_value_list_t::const_iterator itEnd = itKvList->second.end();
					for(it = itKvList->second.begin(); it != itEnd; ++it)
					{
						dobj->add( it->first, deserialize_attribute<H>(it->second) );
					}

					attributes.erase(itKvList);
				}

				return attribute_deserializer<T>::execute(attributes, dobj);
			}
			//--------------------------------------------------------------------------------------

			//--------------------------------------------------------------------------------------
			template<typename T>
			static T deserialize_attribute(const std::string &strVal)
			{
				T val;
				std::stringstream ss(strVal);
				ss >> val;
				return val;
			}

			//--------------------------------------------------------------------------------------
			template<>
			static bool deserialize_attribute<bool>(const std::string &strVal)
			{
				return string_utils::to_lower_case(strVal) == "true";
			}
			//--------------------------------------------------------------------------------------
			template<>
			static std::string deserialize_attribute<std::string>(const std::string &strVal)
			{
				return  strVal.substr(1, strVal.size() - 2);
			}
			//--------------------------------------------------------------------------------------


			//--------------------------------------------------------------------------------------
			static bool execute(const std::string &attributeName, const rapidjson::Value &attributeValue, data_object_sptr dobj)
			{
				if( json_serializer<H>::is_of_type(attributeValue) )
				{
					H val;
					json_serializer<H>::deserialize(attributeValue, val);
					dobj->add(attributeName, val);
					return true;
				}

				return attribute_deserializer<T>::execute(attributeName, attributeValue, dobj);
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
			static bool execute(filesystem::stream_sptr strm, const data_object_sptr &dobj)
			{
				return true;
			}
		};
		//------------------------------------------------------------------------------------------
		template<typename H, typename T>
		struct attribute_serializer< utils::typelist<H,T> >
		{
			//--------------------------------------------------------------------------------------
			static bool execute(filesystem::stream_sptr strm, const data_object_sptr &dobj)
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

			//--------------------------------------------------------------------------------------
			template<typename T>
			static std::string serialize_attribute(const T &val)
			{
				std::stringstream ss;
				ss << val;
				return ss.str();
			}

			//--------------------------------------------------------------------------------------
			template<>
			static std::string serialize_attribute<bool>(const bool &val)
			{
				return val ? "true" : "false";
			}
			//--------------------------------------------------------------------------------------
			template<>
			static std::string serialize_attribute<std::string>(const std::string &val)
			{
				return std::string("\"") + val + std::string("\"");
			}
			//--------------------------------------------------------------------------------------
		};
		//------------------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------------------
		static bool serialize(filesystem::stream_sptr strm, const data_object_sptr &dobj)
		{
			return attribute_serializer<AttributeTypes>::execute(strm, dobj);
		}
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		static bool deserialize(filesystem::stream &stream, data_object_sptr dobj)
		{
			filesystem::memory_stream memStream(&stream);
			const std::string &stringStream = memStream.toString();

			string_utils::string_list_t lines;
			string_utils::split_string(stringStream, lines, "\n");
			
			attribute_set_t attributeSet;

			auto itEnd = lines.end();
			for(auto it = lines.begin(); it != itEnd; ++it)
			{
				const std::string &trimmedLine = string_utils::trimmed( *it );
				
				// Don't care about sections nor comments
				if( !trimmedLine.empty() && trimmedLine[0] != '[' && trimmedLine[0] != '#' )
				{
					std::string::size_type spacePos = trimmedLine.find_first_of(' ');
					std::string::size_type equalPos = trimmedLine.find_first_of('=');

					if( spacePos != std::string::npos && equalPos != std::string::npos )
					{
						const std::string &typeName = trimmedLine.substr(0, spacePos);
						const std::string &attributeName = string_utils::trimmed( trimmedLine.substr(spacePos+1, equalPos - spacePos - 1) );
						const std::string &attributeValue = string_utils::trimmed( trimmedLine.substr(equalPos+1) );

						attributeSet[typeName].push_back( key_value_t(attributeName, attributeValue) );
					}
				}
			}

			attribute_deserializer<AttributeTypes>::execute(attributeSet, dobj);

			return true;
		}
		//------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------------------
		static bool deserialize2(filesystem::stream &stream, data_object_sptr dobj)
		{
			filesystem::memory_stream memStrm(&stream);

			rapidjson::Document pDoc;
			pDoc.Parse<0>(memStrm.toString().c_str());

			if( ensure(!pDoc.HasParseError()) )
			{
				auto itEnd = pDoc.MemberEnd();
				for(auto it = pDoc.MemberBegin(); it != itEnd; ++it)
				{
					const std::string &attributeName = it->name.GetString();
					attribute_deserializer<AttributeTypes>::execute(attributeName, it->value, dobj);
				}
			}

			return true;
		}
		//------------------------------------------------------------------------------------------
	};

} /*resources*/ } /*djah*/

#endif /* DJAH_DATA_OBJECT_INI_SERIALIZER_HPP */