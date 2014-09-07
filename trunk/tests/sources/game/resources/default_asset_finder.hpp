#ifndef GAME_RESOURCES_DEFAULT_ASSET_FINDER_HPP
#define GAME_RESOURCES_DEFAULT_ASSET_FINDER_HPP

#include "djah/core/typelist.hpp"

#include "game/resources/image_loader.hpp"
#include "game/resources/mesh_loader.hpp"
#include "game/resources/data_object_loader.hpp"

#include "djah/resources/loader.hpp"

DJAH_DECLARE_LOADER(game::resources::image);
DJAH_DECLARE_LOADER(game::resources::mesh);
DJAH_DECLARE_LOADER(game::resources::data_object);

#include "djah/resources/asset_finder.hpp"

namespace game { namespace resources {

    typedef TYPELIST(image, mesh, data_object) DefaultAssetsTypes;
    typedef djah::resources::asset_finder<DefaultAssetsTypes> default_asset_finder;

} /*resources*/ } /*game*/

#endif /* GAME_RESOURCES_DEFAULT_ASSET_FINDER_HPP */