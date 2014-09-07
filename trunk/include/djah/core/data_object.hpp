#ifndef DJAH_CORE_DATA_OBJECT_HPP
#define DJAH_CORE_DATA_OBJECT_HPP

#include <map>
#include <string>
#include <ostream>
#include <sstream>
#include "djah/debug/assertion.hpp"
#include "djah/core/typelist.hpp"
#include "djah/core/hierarchy_generation.hpp"

namespace djah {

    //----------------------------------------------------------------------------------------------
    // An attributes visitor enables the iteration on all the attributes of all types
    template<typename AttributeTypes>
    struct attributes_visitor;
    //----------------------------------------------------------------------------------------------
    template<>
    struct attributes_visitor<nulltype>
    {
        template<typename DO>
        static bool has(const DO&, const std::string&) { return false; }

        template<typename DO>
        static std::string to_string(const DO&) { return std::string(); }
    };
    //----------------------------------------------------------------------------------------------
    template<typename HeadAttribute, typename TailList>
    struct attributes_visitor< typelist<HeadAttribute, TailList> >
    {
        template<typename DO>
        static bool has(const DO &dobj, const std::string &attributeName)
        {
            return dobj.has<HeadAttribute>(attributeName)
                || attributes_visitor<TailList>::has(dobj, attributeName);
        }

        template<typename DO>
        static std::string to_string(const DO &dobj)
        {
            std::stringstream result;
            auto _attributes = dobj.attributes<HeadAttribute>();
            auto itEnd = _attributes.end();
            for(auto it = _attributes.begin(); it != itEnd; ++it)
            {
                result << it->first << " = " << it->second << "\n";
            }

            return result.str() + attributes_visitor<TailList>::to_string(dobj);
        }
    };
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // An attribute holder holds all attributes of one type for one data object
    template<typename T>
    struct attributes_holder
    {
        typedef std::map<std::string, T> attribute_map_t;
        attribute_map_t attributes_;
    };
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // A data object is a collection of attributes of different types
    template<typename AttributeTypes>
    class data_object_t
        : public gen_scatter_hierarchy<AttributeTypes, attributes_holder>
    {
    public:
        //------------------------------------------------------------------------------------------
        template<typename T>
        const typename attributes_holder<T>::attribute_map_t& attributes() const
        {
            return attributes_holder<T>::attributes_;
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        template<typename T>
        T get(const std::string &attributeName, const T &defaultValue = T()) const
        {
            T result = defaultValue;

            auto it = attributes_holder<T>::attributes_.find(attributeName);
            if( it != attributes_holder<T>::attributes_.end() )
                result = it->second;

            return result;
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        template<typename T>
        void set(const std::string &attributeName, const T &value)
        {
            auto it = attributes_holder<T>::attributes_.find(attributeName);
            if( it != attributes_holder<T>::attributes_.end() )
                it->second = value;
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        template<typename T>
        bool has(const std::string &attributeName) const
        {
            auto it = attributes_holder<T>::attributes_.find(attributeName);
            return (it != attributes_holder<T>::attributes_.end());
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        bool has(const std::string &attributeName) const
        {
            return attributes_visitor<AttributeTypes>::has(*this, attributeName);
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        template<typename T>
        void add(const std::string &attrName, const T &attrValue)
        {
            DJAH_ASSERT_MSG( !has<T>(attrName), "data_object::add(%s), trying to add an already existing attribute", attrName.c_str() );
            attributes_holder<T>::attributes_[attrName] = attrValue;
        }
        //----------------------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------------------
        std::string toString() const
        {
            return attributes_visitor<AttributeTypes>::to_string(*this);
        }
        //----------------------------------------------------------------------------------------------
    };
    //----------------------------------------------------------------------------------------------

    template<typename AttributeTypes>
    std::ostream& operator <<(std::ostream &out, const data_object_t<AttributeTypes> &dataObject)
    {
        return out << dataObject.toString();
    }

} /*djah*/

#endif /* DJAH_CORE_DATA_OBJECT_HPP */