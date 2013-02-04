#ifndef DJAH_RESOURCES_MESH_LOADER_HPP
#define DJAH_RESOURCES_MESH_LOADER_HPP

#include "loader.hpp"
#include "mesh.hpp"

namespace djah { namespace resources {

	class mesh_loader
		: public loader<mesh>
	{
	public:
		mesh_loader();
		virtual ~mesh_loader();
		mesh* loadFromStream(filesystem::stream &stream, const std::string &fileName = "");

	private:
		submesh* loadSubMesh(filesystem::stream &stream);
	};

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_MESH_LOADER_HPP */