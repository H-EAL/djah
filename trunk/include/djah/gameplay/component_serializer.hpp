#ifndef DJAH_GAMEPLAY_COMPONENT_SERIALIZER_HPP
#define DJAH_GAMEPLAY_COMPONENT_SERIALIZER_HPP

#include <map>
#include <string>
#include <fstream>
#include <cstdio>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/prettywriter.h"

#include "djah/core/typelist.hpp"

#include "djah/filesystem/stream.hpp"
#include "djah/filesystem/browser.hpp"
#include "djah/filesystem/memory_stream.hpp"


namespace djah { namespace gameplay {

    //----------------------------------------------------------------------------------------------
    template<typename ComponentsTypeList>
    class entity_t;
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    template<typename ComponentsTypeList>
    struct component_data_aux
    {
        entity_t<ComponentsTypeList>	*pGO;
        rapidjson::Value			*pValue;
    };
    //----------------------------------------------------------------------------------------------
    template<typename ComponentsTypeList>
    struct component_data
    {
        std::vector< component_data_aux<ComponentsTypeList> > data;
    };
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    template<typename ComponentsTypeList>
    struct component_serializer_visitor;
    //----------------------------------------------------------------------------------------------
    template<>
    struct component_serializer_visitor<nulltype>
    {
        template<typename Entity>
        static void serialize(rapidjson::Document&, const Entity&) {}

        template<typename ComponentsTypeList>
        static void deserialize(std::map< std::string, component_data<ComponentsTypeList> >&) {}
    };
    //----------------------------------------------------------------------------------------------
    template<typename HeadComponent, typename TailComponents>
    struct component_serializer_visitor< typelist<HeadComponent,TailComponents> >
    {
        template<typename Entity>
        static void serialize(rapidjson::Document &document, const Entity &go)
        {
            if( go.isUsing<HeadComponent>() )
            {
                rapidjson::Value componentNode;
                componentNode.SetObject();

                go.get<HeadComponent>()->serialize(document, componentNode);

                document.AddMember(HeadComponent::name(), componentNode, document.GetAllocator());
            }
            component_serializer_visitor<TailComponents>::serialize(document, go);
        }

        template<typename ComponentsTypeList>
        static void deserialize(std::map< std::string, component_data<ComponentsTypeList> > &compMap)
        {
            auto compData = compMap.find(HeadComponent::name());
            if( compData != compMap.end() )
            {
                auto itEnd = compData->second.data.end();
                for(auto it = compData->second.data.begin(); it != itEnd; ++it)
                {
                    entity_t<ComponentsTypeList> &go = *(it->pGO);

                    if( go.isUsing<HeadComponent>() )
                    {
                        component<HeadComponent> &comp = go.get<HeadComponent>();
                        *comp = HeadComponent(*(it->pValue));
                    }
                    else
                    {
                        go.use( HeadComponent(*(it->pValue)) );
                    }
                }
                compMap.erase(compData);
            }

            component_serializer_visitor<TailComponents>::deserialize(compMap);
        }
    };
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    template<typename ComponentsTypeList>
    class component_serializer
    {
    public:
        typedef entity_t<ComponentsTypeList> Entity;
        typedef std::map< std::string, component_data<ComponentsTypeList> > comp_map_t;

        static void serialize(const Entity &go)
        {
            const std::string &fileName = go.name();
            filesystem::stream_sptr strm = filesystem::browser::get().openWriteStream(fileName);
            if( strm )
            {
                rapidjson::Document doc;
                doc.SetObject();
                component_serializer_visitor<ComponentsTypeList>::serialize(doc, go);

                rapidjson::FileStream f(stdout);
                struct JSON_Writer
                {
                    JSON_Writer(filesystem::stream &_stream) : stream_(_stream) {}
                    void Put(char c) { stream_ << c; }
                    filesystem::stream &stream_;
                } w(*strm);
                rapidjson::PrettyWriter<JSON_Writer> writer(w);
                doc.Accept(writer);
            }
        }

        static void serialize(std::vector<Entity*> &goList)
        {
            auto itEnd = goList.end();
            for(auto it = goList.begin(); it != itEnd; ++it)
            {
                serialize(*(*it));
            }
        }

        static void deserialize(Entity &go)
        {
            comp_map_t compMap;

            deserialize(go, compMap );

            component_serializer_visitor<ComponentsTypeList>::deserialize(compMap);
            clearDocStack();
        }

        static void deserialize(std::vector<Entity*> &goList)
        {
            comp_map_t compMap;

            auto itEnd = goList.end();
            for(auto it = goList.begin(); it != itEnd; ++it)
            {
                deserialize( *(*it), compMap );
            }

            component_serializer_visitor<ComponentsTypeList>::deserialize(compMap);
            clearDocStack();
        }

        static void deserialize(Entity &entity, rapidjson::Value &entityNode)
        {
            comp_map_t compMap;

            deserialize(entity, entityNode, compMap);

            component_serializer_visitor<ComponentsTypeList>::deserialize(compMap);
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

        static void deserialize(Entity &entity, comp_map_t &compMap)
        {
            const std::string &fileName = entity.name();
            filesystem::stream_sptr strm = filesystem::browser::get().openReadStream(fileName);

            check(strm && strm->size() > 0);
            filesystem::memory_stream memStrm(strm.get());

            rapidjson::Document *pDoc = new rapidjson::Document;
            docStack_.push(pDoc);
            pDoc->Parse<0>(memStrm.toString().c_str());

            if( ensure(!pDoc->HasParseError()) )
            {
                //go.stopUsingAll();

                deserialize(entity, *pDoc, compMap);
            }
        }

        static void deserialize(Entity &entity, rapidjson::Value &entityNode, comp_map_t &compMap)
        {
            auto itEnd = entityNode.MemberEnd();
            for(auto it = entityNode.MemberBegin(); it != itEnd; ++it)
            {
                const std::string &compName = it->name.GetString();
                component_data_aux<ComponentsTypeList> compData = {&entity, &(it->value)};
                compMap[compName].data.push_back( compData );
            }
        }

        static std::stack<rapidjson::Document*> docStack_;
    };
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    template<typename ComponentsTypeList>
    std::stack<rapidjson::Document*> component_serializer<ComponentsTypeList>::docStack_;
    //----------------------------------------------------------------------------------------------

} /*gameplay*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_COMPONENT_SERIALIZER_HPP */