#ifndef GAME_RESOURCES_DEFAULT_DATA_OBJECT_HPP
#define GAME_RESOURCES_DEFAULT_DATA_OBJECT_HPP

#include <memory>

#include "djah/core/data_object.hpp"
#include "djah/core/typelist.hpp"
#include "djah/resources/asset.hpp"

#include "djah/math/vector3.hpp"
#include "djah/math/quaternion.hpp"

#include "game/components/enum_test.hpp"

namespace game { namespace resources {
    
    //----------------------------------------------------------------------------------------------
    // List of supported types
    typedef ENUM(TextureFilter) ETextureFilter;
    typedef TYPELIST
    (
        int,
        float,
        bool,
        double,
        std::string,
        djah::math::vector3f,
        djah::math::quatf,
        ETextureFilter
    )
    DefaultAttributeTypes;
    //----------------------------------------------------------------------------------------------

    class data_object
        : public djah::data_object_t<DefaultAttributeTypes>
        , public djah::resources::asset
    {
    public:
        data_object(const std::string &_name)
            : djah::resources::asset(_name)
        {}
    };

    typedef std::shared_ptr<data_object> data_object_sptr;

} /*resources*/ } /*game*/

#endif /* GAME_RESOURCES_DEFAULT_DATA_OBJECT_HPP */