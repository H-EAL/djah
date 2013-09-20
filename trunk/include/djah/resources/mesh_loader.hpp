#ifndef DJAH_RESOURCES_MESH_LOADER_HPP
#define DJAH_RESOURCES_MESH_LOADER_HPP

#include "djah/filesystem/stream.hpp"
#include "djah/resources/mesh.hpp"

namespace djah { namespace resources {

	class mesh_loader
	{
	public:
		static mesh_sptr loadFromStream(filesystem::stream &stream, const std::string &url = "");

	private:
		static submesh* loadSubMesh(filesystem::stream &stream);
	};

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_MESH_LOADER_HPP */