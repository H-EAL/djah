#ifndef DJAH_RESOURCES_DATA_OBJECT_HPP
#define DJAH_RESOURCES_DATA_OBJECT_HPP

#include <map>
#include <string>
#include <memory>
#include <ostream>
#include <sstream>
#include "djah/debug/assertion.hpp"
#include "djah/core/enum.hpp"
#include "djah/core/typelist.hpp"
#include "djah/core/hierarchy_generation.hpp"
#include "djah/resources/asset.hpp"
#include "djah/math/vector3.hpp"
#include "djah/math/quaternion.hpp"
#include "djah/debug/log.hpp"
#include "djah/game/components/enum_test.hpp"

namespace djah { namespace resources {

    //----------------------------------------------------------------------------------------------
    // An attribute hook enables the iteration on all the attributes of all types
    template<typename AttributeTypes>
    struct attribute_hook;
    //----------------------------------------------------------------------------------------------
    template<>
    struct attribute_hook<utils::nulltype>
    {
        template<typename DO>
        static bool has(const DO&, const std::string&) { return false; }

        template<typename DO>
        static std::string to_string(const DO&) { return std::string(); }
    };
    //----------------------------------------------------------------------------------------------
    template<typename HeadAttribute, typename TailList>
    struct attribute_hook< utils::typelist<HeadAttribute, TailList> >
    {
        template<typename DO>
        static bool has(const DO &dobj, const std::string &attributeName)
        {
            return dobj.has<HeadAttribute>(attributeName)
                || attribute_hook<TailList>::has(dobj, attributeName);
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

            return result.str() + attribute_hook<TailList>::to_string(dobj);
        }
    };
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // An attribute holder holds all attributes of one type for one data object
    template<typename T>
    struct attribute_holder
    {
        typedef std::map<std::string, T> attribute_map_t;
        attribute_map_t attributes_;
    };
    //----------------------------------------------------------------------------------------------


    typedef ENUM(TextureFilter) ETextureFilter;
    //----------------------------------------------------------------------------------------------
    // List of supported types
    typedef TYPELIST
    (
        int, 
        float, 
        bool, 
        double, 
        std::string, 
        math::vector3f, 
        math::quatf,
        ETextureFilter
    )
    DefaultAttributeTypes;
    //----------------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------------
    // A data object is a collection of attributes of different types
    template<typename AttributeTypes = DefaultAttributeTypes>
    class data_object
        : public utils::gen_scatter_hierarchy<AttributeTypes, attribute_holder>
        , public asset
    {
    public:
        typedef data_object<AttributeTypes>	   data_object_t;
        typedef std::shared_ptr<data_object_t> data_object_sptr;
        typedef std::weak_ptr<data_object_t>   data_object_wptr;

    public:
        //------------------------------------------------------------------------------------------
        data_object(const std::string &name)
            : asset(name)
        {
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        template<typename T>
        const typename attribute_holder<T>::attribute_map_t& attributes() const
        {
            return attribute_holder<T>::attributes_;
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        template<typename T>
        T get(const std::string &attributeName, const T &defaultValue = T()) const
        {
            T result = defaultValue;

            auto it = attribute_holder<T>::attributes_.find(attributeName);
            if( it != attribute_holder<T>::attributes_.end() )
                result = it->second;

            return result;
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        template<typename T>
        void set(const std::string &attributeName, const T &value)
        {
            auto it = attribute_holder<T>::attributes_.find(attributeName);
            if( it != attribute_holder<T>::attributes_.end() )
                it->second = value;
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        template<typename T>
        bool has(const std::string &attributeName) const
        {
            auto it = attribute_holder<T>::attributes_.find(attributeName);
            return (it != attribute_holder<T>::attributes_.end());
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        bool has(const std::string &attributeName) const
        {
            return attribute_hook<AttributeTypes>::has(*this, attributeName);
        }
        //------------------------------------------------------------------------------------------

        //------------------------------------------------------------------------------------------
        template<typename T>
        void add(const std::string &attrName, const T &attrValue)
        {
            DJAH_ASSERT_MSG( !has<T>(attrName), "data_object[%s]::add(%s), trying to add an already existing attribute", name_.c_str(), attrName.c_str() );
            attribute_holder<T>::attributes_[attrName] = attrValue;
        }
        //----------------------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------------------
        std::string toString() const
        {
            return attribute_hook<AttributeTypes>::to_string(*this);
        }
        //----------------------------------------------------------------------------------------------
    };
    //----------------------------------------------------------------------------------------------

    template<typename AttributeTypes>
    std::ostream& operator <<(std::ostream &out, const data_object<AttributeTypes> &dataObject)
    {
        return out << dataObject.toString();
    }

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_DATA_OBJECT_HPP */