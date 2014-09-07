#ifndef GAME_RESOURCES_MESH_LOADER_HPP
#define GAME_RESOURCES_MESH_LOADER_HPP

#include "djah/filesystem/stream.hpp"
#include "game/resources/mesh.hpp"

namespace game { namespace resources {

	class mesh_loader
	{
	public:
        static bool loadFromStream(djah::filesystem::stream &stream, const std::string &url, mesh_sptr &spMesh);

	private:
        static submesh* loadSubMesh(djah::filesystem::stream &stream);
	};

} /*resources*/ } /*game*/

#endif /* GAME_RESOURCES_MESH_LOADER_HPP */