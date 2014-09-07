#ifndef GAME_RESOURCES_IMAGE_LOADER_HPP
#define GAME_RESOURCES_IMAGE_LOADER_HPP

#include "djah/filesystem/stream.hpp"
#include "game/resources/image.hpp"

namespace game { namespace resources {

	class image_loader
	{
	public:
        static bool loadFromStream(djah::filesystem::stream &stream, const std::string &url, image_sptr &spImage);
	};

} /*resources*/ } /*game*/

#endif /* GAME_RESOURCES_IMAGE_LOADER_HPP */