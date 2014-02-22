#ifndef DJAH_GAMEPLAY_COMPONENT_SERIALIZER_HPP
#define DJAH_GAMEPLAY_COMPONENT_SERIALIZER_HPP

#include <map>
#include <string>
#include <fstream>
#include <cstdio>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

#include "djah/core/typelist.hpp"

#include "djah/filesystem/stream.hpp"
#include "djah/filesystem/browser.hpp"
#include "djah/filesystem/memory_stream.hpp"


namespace djah { namespace gameplay {

	//----------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	class game_object;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	struct component_data_aux
	{
		game_object<ComponentTypeList>	*pGO;
		rapidjson::Value				*pValue;
	};
	//----------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	struct component_data
	{
		std::vector< component_data_aux<ComponentTypeList> > data;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	struct component_serializer;
	//----------------------------------------------------------------------------------------------
	template<>
	struct component_serializer<utils::nulltype>
	{
		template<typename ComponentTypeList>
		static void serialize(game_object<ComponentTypeList> &, filesystem::stream&, bool) {}

		template<typename ComponentTypeList>
		static void deserialize(const std::map< std::string, component_data<ComponentTypeList> >&) {}
	};
	//----------------------------------------------------------------------------------------------
	template<typename HeadComponent, typename TailComponents>
	struct component_serializer< utils::typelist<HeadComponent,TailComponents> >
	{
		template<typename ComponentTypeList>
		static void serialize(game_object<ComponentTypeList> &go, filesystem::stream_sptr &strm, bool first = true)
		{
			if( go.isUsing<HeadComponent>() )
			{
				if( !first )
				{
					strm << ", ";
				}
				first = false;
				go.getData<HeadComponent>().serialize(strm);
			}
			component_serializer<TailComponents>::serialize(go, strm, first);
		}

		template<typename ComponentTypeList>
		static void deserialize(const std::map< std::string, component_data<ComponentTypeList> > &compMap)
		{
			auto compData = compMap.find(HeadComponent::name());
			if( compData != compMap.end() )
			{
				auto itEnd = compData->second.data.end();
				for(auto it = compData->second.data.begin(); it != itEnd; ++it)
				{
					game_object<ComponentTypeList> &go = *(it->pGO);
					go.use( HeadComponent(*(it->pValue)) );
				}
			}
			component_serializer<TailComponents>::deserialize(compMap);
		}
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	class game_object_serializer
	{
	public:
		typedef game_object<ComponentTypeList> game_object_t;
		typedef std::map< std::string, component_data<ComponentTypeList> > comp_map_t;

		static void serialize(const game_object_t &go)
		{
			const std::string &fileName = go.name() + ".json";
			filesystem::stream_sptr strm = filesystem::browser::get().openWriteStream(fileName);
			if( strm )
			{
				strm << "{ ";
				component_serializer<ComponentTypeList>::serialize(go, strm);
				strm << " }";
			}
		}

		static void serialize(std::vector<game_object_t*> &goList)
		{
			auto itEnd = goList.end();
			for(auto it = goList.begin(); it != itEnd; ++it)
			{
				serialize(*(*it));
			}
		}

		static void deserialize(game_object_t &go)
		{
			comp_map_t compMap;

			deserialize(go, compMap );

			component_serializer<ComponentTypeList>::deserialize(compMap);
			clearDocStack();
		}

		static void deserialize(std::vector<game_object_t*> &goList)
		{
			comp_map_t compMap;

			auto itEnd = goList.end();
			for(auto it = goList.begin(); it != itEnd; ++it)
			{
				deserialize( *(*it), compMap );
			}

			component_serializer<ComponentTypeList>::deserialize(compMap);
			clearDocStack();
		}

	private:
		static void clearDocStack()
		{
			while( !docStack_.empty() )
			{
				rapidjson::Document *pDoc = docStack_.top();
				docStack_.pop();
				delete pDoc;
			}
		}

		static void deserialize(game_object_t &go, comp_map_t &compMap)
		{
			const std::string &fileName = go.name() + ".json";
			filesystem::stream_sptr strm = filesystem::browser::get().openReadStream(fileName);

			check(strm && strm->size() > 0);
			filesystem::memory_stream memStrm(strm.get());

			go.stopUsingAll();

			rapidjson::Document *pDoc = new rapidjson::Document;
			docStack_.push(pDoc);
			pDoc->Parse<0>(memStrm.toString().c_str());

			if( ensure(!pDoc->HasParseError()) )
			{
				auto itEnd = pDoc->MemberEnd();
				for(auto it = pDoc->MemberBegin(); it != itEnd; ++it)
				{
					const std::string &compName = it->name.GetString();
					component_data_aux<ComponentTypeList> compData = {&go, &(it->value)};
					compMap[compName].data.push_back( compData );
				}
			}
		}

		static std::stack<rapidjson::Document*> docStack_;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	template<typename ComponentTypeList>
	std::stack<rapidjson::Document*> game_object_serializer<ComponentTypeList>::docStack_;
	//----------------------------------------------------------------------------------------------

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENT_SERIALIZER_HPP */