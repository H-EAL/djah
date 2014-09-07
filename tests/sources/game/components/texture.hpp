#ifndef GAME_COMPONENTS_TEXTURE_HPP
#define GAME_COMPONENTS_TEXTURE_HPP

#include "djah/gameplay/component.hpp"
#include "game/components/enum_test.hpp"
#include "../../resource_finder.hpp"

namespace game { namespace components {

    struct texture
    {
        MAKE_COMPONENT(texture);

        // serializable
        std::string file;

        // transient
        djah::d3d::texture_sptr spTexture;
    };

    struct uv_modifier
    {
        MAKE_COMPONENT(uv_modifier);

        // serializable
        djah::math::vector2f scale;
        djah::math::vector2f offset;
        djah::math::vector2f velocity;
        ENUM(TextureFilter)  textureFilter;

        // transient
        djah::math::vector2f position;
    };

} /*components*/ } /*game*/

#endif /* GAME_COMPONENTS_TEXTURE_HPP */