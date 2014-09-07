#ifndef DJAH_GAMEPLAY_JSON_SERIALIZER_HPP
#define DJAH_GAMEPLAY_JSON_SERIALIZER_HPP

#include <map>
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "djah/math.hpp"
#include "djah/core/enum.hpp"
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
    void json_serialize(rapidjson::Document &document, rapidjson::Value &componentNode, const std::string &name, const T &val)
    {
        rapidjson::Value nameNode;
        nameNode.SetString(name.c_str(), name.size(), document.GetAllocator());

        rapidjson::Value valueNode;
        json_serializer<T>::serialize(document, valueNode, val);

        componentNode.AddMember(nameNode, valueNode, document.GetAllocator());
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
            return node.IsDouble() || node.IsInt();
        }

        static void deserialize(const rapidjson::Value &node, float &val)
        {
            check(is_of_type(node));
            val = float(node.GetDouble());
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, float val)
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
            return node.IsDouble() || node.IsInt();
        }

        static void deserialize(const rapidjson::Value &node, double &val)
        {
            check(is_of_type(node));
            val = node.GetDouble();
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, double val)
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
            check(is_of_type(node));
            val = node.GetBool();
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, bool val)
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
            check(is_of_type(node));
            val = node.GetInt();
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, int val)
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
            check(is_of_type(node));
            val = node.GetUint();
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, unsigned int val)
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
            check(is_of_type(node));
            val = std::string(node.GetString(), node.GetStringLength());
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, const std::string &val)
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
            check(is_of_type(node));

            for(rapidjson::SizeType i = 0; i < N; ++i)
            {
                json_serializer<T>::deserialize(node[i], val[i]);
            }
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, const math::vector<N,T> &val)
        {
            node.SetArray();
            node.Reserve(N, document.GetAllocator());

            for(rapidjson::SizeType i = 0; i < N; ++i)
            {
                rapidjson::Value valueNode;
                json_serializer<T>::serialize(document, valueNode, val[i]);
                node.PushBack(valueNode, document.GetAllocator());
            }
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
            check(is_of_type(node));

            for(rapidjson::SizeType i = 0; i < 4; ++i)
            {
                json_serializer<T>::deserialize(node[i], val[i]);
            }
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, const math::quaternion<T> &val)
        {
            node.SetArray();
            node.Reserve(4, document.GetAllocator());

            for(rapidjson::SizeType i = 0; i < 4; ++i)
            {
                rapidjson::Value valueNode;
                json_serializer<T>::serialize(document, valueNode, val[i]);
                node.PushBack(valueNode, document.GetAllocator());
            }
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
                result = node[i].IsArray() && node[i].Size() == M;

                for(rapidjson::SizeType j = 0; j < M && result; ++j)
                {
                    result = json_serializer<T>::is_of_type(node[i][j]);
                }
            }

            return result;
        }

        static void deserialize(const rapidjson::Value &node, math::matrix<N,M,T> &val)
        {
            check(is_of_type(node));

            for(rapidjson::SizeType i = 0; i < N; ++i)
            {
                for(rapidjson::SizeType j = 0; j < M; ++j)
                {
                    json_serializer<T>::deserialize(node[i][j], val[i][j]);
                }
            }
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, const math::matrix<N,M,T> &val)
        {
            node.SetArray();
            node.Reserve(N, document.GetAllocator());

            for(rapidjson::SizeType i = 0; i < N; ++i)
            {
                rapidjson::Value rowNode;
                rowNode.SetArray();
                rowNode.Reserve(M);

                for(rapidjson::SizeType j = 0; j < M; ++j)
                {
                    rapidjson::Value valueNode;
                    json_serializer<T>::serialize(document, valueNode, val[i][j]);
                    rowNode.PushBack(valueNode, document.GetAllocator());
                }

                node.PushBack(rowNode, document.GetAllocator());
            }
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
            check(is_of_type(node));

            const rapidjson::SizeType vecSize = node.Size();
            val.clear();
            val.resize(vecSize);
            for(rapidjson::SizeType i = 0; i < vecSize; ++i)
            {
                json_serializer<T>::deserialize(node[i], val[i]);
            }
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, const std::vector<T> &val)
        {
            const rapidjson::SizeType vecSize = val.size();

            node.SetArray();
            node.Reserve(vecSize, document.GetAllocator());

            for(rapidjson::SizeType i = 0; i < vecSize; ++i)
            {
                rapidjson::Value valueNode;
                json_serializer<T>::serialize(document, valueNode, val[i]);
                node.PushBack(valueNode, document.GetAllocator());
            }
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
            check(is_of_type(node));

            val.clear();

            const auto itEnd = node.MemberEnd();
            for(auto it = node.MemberBegin(); it != itEnd; ++it)
            {
                const std::string &key = it->name.GetString();

                if( val.find(key) != val.end() )
                {
                    DJAH_GLOBAL_WARNING()
                        << "Duplicate keys (" << key << ") found in attribute \""
                        << it->name.GetString() << "\", the value will be overwritten"
                        << DJAH_END_LOG();
                }
                // Duplicate keys are overwritten
                json_serializer<T>::deserialize(it->value, val[key]);
            }
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, const std::map<std::string,T> &val)
        {
            node.SetObject();
            std::for_each(val.begin(), val.end(), [&](const std::map<std::string,T>::value_type &entry)
            {
                rapidjson::Value keyNode;
                keyNode.SetString(entry.first.c_str());

                rapidjson::Value valueNode;
                json_serializer<T>::serialize(document, valueNode, entry.second);

                node.AddMember(keyNode, valueNode, document.GetAllocator());
            });
        }
    };
    //----------------------------------------------------------------------------------------------
    template<typename EnumType, typename EnumDescriptorType>
    struct json_serializer< enum_t<EnumType, EnumDescriptorType> >
    {
        static bool is_of_type(const rapidjson::Value &node)
        {
            return node.IsArray() && node.Size() == 1 && node[rapidjson::SizeType(0)].IsString();
        }

        static void deserialize(const rapidjson::Value &node, enum_t<EnumType, EnumDescriptorType> &val)
        {
            check(is_of_type(node));
            val = std::string(node[rapidjson::SizeType(0)].GetString(), node[rapidjson::SizeType(0)].GetStringLength());
        }

        static void serialize(rapidjson::Document &document, rapidjson::Value &node, enum_t<EnumType, EnumDescriptorType> val)
        {
            node.SetArray();
            node.Reserve(1, document.GetAllocator());
            rapidjson::Value valueNode;
            valueNode.SetString(val.toString());
            node.PushBack(valueNode, document.GetAllocator());
        }
    };

} /*resources*/ } /*djah*/

#endif /* DJAH_GAMEPLAY_JSON_SERIALIZER_HPP */