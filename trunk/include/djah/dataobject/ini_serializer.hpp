#ifndef DJAH_DATA_OBJECT_INI_SERIALIZER_HPP
#define DJAH_DATA_OBJECT_INI_SERIALIZER_HPP

#include <map>
#include <memory>
#include "data_object.hpp"
#include "serializer.hpp"
#include "../filesystem/stream.hpp"
#include "../filesystem/memory_stream.hpp"
#include "../core/string_utils.hpp"

namespace djah { namespace dataobject {

	template<typename AttributeTypes>
	class ini_serializer
	{
	private:
		typedef data_object<AttributeTypes> data_object_t;
		typedef std::shared_ptr<data_object_t> data_object_ptr;

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
			static bool execute(attribute_set_t &attributes, data_object_ptr)
			{
				return attributes.empty();
			}
		};
		//------------------------------------------------------------------------------------------
		template<typename H, typename T>
		struct attribute_deserializer< utils::typelist<H,T> >
		{
			//--------------------------------------------------------------------------------------
			static bool execute(attribute_set_t &attributes, data_object_ptr dobj)
			{
				attribute_set_t::const_iterator itKvList = attributes.find( type_name<H>::value() );
				if( itKvList != attributes.end() )
				{
					key_value_list_t::const_iterator it;
					key_value_list_t::const_iterator itEnd = itKvList->second.end();
					for(it = itKvList->second.begin(); it != itEnd; ++it)
					{
						dobj->add( deserialize_attribute<H>(*it) );
					}

					attributes.erase(itKvList);
				}

				return attribute_deserializer<T>::execute(attributes, dobj);
			}
			//--------------------------------------------------------------------------------------

			//--------------------------------------------------------------------------------------
			template<typename T>
			static attribute<T> deserialize_attribute(const key_value_t &kv)
			{
				T val;
				std::stringstream ss(kv.second);
				ss >> val;
				return attribute<T>(kv.first, val);
			}

			//--------------------------------------------------------------------------------------
			template<>
			static attribute<bool> deserialize_attribute<bool>(const key_value_t &kv)
			{
				return attribute<bool>(kv.first, utils::to_lower_case(kv.second) == "true");
			}
			//--------------------------------------------------------------------------------------
			template<>
			static attribute<std::string> deserialize_attribute<std::string>(const key_value_t &kv)
			{
				return attribute<std::string>( kv.first, kv.second.substr(1, kv.second.size() - 2) );
			}
			//--------------------------------------------------------------------------------------
			template<>
			static attribute<math::vector3f> deserialize_attribute<math::vector3f>(const key_value_t &kv)
			{
				math::vector3f v;
				std::stringstream ss(kv.second);
				ss >> v.x >> v.y >> v.z;
				return attribute<math::vector3f>(kv.first, v);
			}
			//--------------------------------------------------------------------------------------
			template<>
			static attribute<math::quatf> deserialize_attribute<math::quatf>(const key_value_t &kv)
			{
				math::quatf q;
				std::stringstream ss(kv.second);
				ss >> q.x >> q.y >> q.z >> q.w;
				return attribute<math::quatf>(kv.first, q);
			}
			//--------------------------------------------------------------------------------------
		};
		//------------------------------------------------------------------------------------------

	public:
		//------------------------------------------------------------------------------------------
		static bool deserialize(filesystem::stream_ptr strm, data_object_ptr dobj)
		{
			filesystem::memory_stream memStream(strm);
			std::string &stringStream = memStream.toString();

			utils::string_list_t lines;
			utils::split_string(stringStream, lines, "\n");
			
			attribute_set_t attributeSet;

			auto itEnd = lines.end();
			for(auto it = lines.begin(); it != itEnd; ++it)
			{
				const std::string &trimmedLine = utils::trimmed( *it );
				
				// Don't care about sections nor comments
				if( !trimmedLine.empty() && trimmedLine[0] != '[' && trimmedLine[0] != '#' )
				{
					std::string::size_type spacePos = trimmedLine.find_first_of(' ');
					std::string::size_type equalPos = trimmedLine.find_first_of('=');

					if( spacePos != std::string::npos && equalPos != std::string::npos )
					{
						const std::string &typeName = trimmedLine.substr(0, spacePos);
						const std::string &attributeName = utils::trimmed( trimmedLine.substr(spacePos+1, equalPos - spacePos - 1) );
						const std::string &attributeValue = utils::trimmed( trimmedLine.substr(equalPos+1) );

						attributeSet[typeName].push_back( key_value_t(attributeName, attributeValue) );
					}
				}
			}

			attribute_deserializer<AttributeTypes>::execute(attributeSet, dobj);

			return true;
		}
		//------------------------------------------------------------------------------------------
	};

} /*dataobject*/ } /*djah*/

#endif /* DJAH_DATA_OBJECT_INI_SERIALIZER_HPP */