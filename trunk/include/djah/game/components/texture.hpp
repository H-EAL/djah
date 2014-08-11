#ifndef DJAH_GAME_COMPONENTS_TEXTURE_HPP
#define DJAH_GAME_COMPONENTS_TEXTURE_HPP

#include "djah/gameplay/component.hpp"
#include "djah/resources/image.hpp"
#include "../../../tests/sources/resource_finder.hpp"
#include "enum_test.hpp"

namespace djah { namespace game { namespace components {

    struct texture
    {
        MAKE_COMPONENT(texture);

        // serializable
        std::string file;

        // transient
        resources::image_sptr spImage;
        d3d::texture_sptr	  spTexture;
    };

    struct uv_modifier
    {
        MAKE_COMPONENT(uv_modifier);

        // serializable
        math::vector2f scale;
        math::vector2f offset;
        math::vector2f velocity;
        ENUM(TextureFilter) textureFilter;

        // transient
        math::vector2f position;
    };

} /*components*/ } /*game*/ } /*djah*/

#endif /* DJAH_GAME_COMPONENTS_TEXTURE_HPP */