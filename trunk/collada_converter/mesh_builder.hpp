#ifndef MESH_BUILDER_HPP
#define MESH_BUILDER_HPP

#include "collada/fwd_decl.hpp"

//--------------------------------------------------------------------------------------------------
class submesh;
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
class mesh_builder
{
public:
	mesh_builder(const collada::proxy &obj);
	~mesh_builder();

	const std::vector<float>& getBuffer(int i) const;
	size_t getBufferCount(int i) const;
	size_t getVertexSize(int i) const;
	size_t getSubMeshesCount() const;

private:
	typedef std::vector<submesh*> submesh_list_t;
	submesh_list_t submeshes_;
};
//--------------------------------------------------------------------------------------------------

#endif /* MESH_BUILDER_HPP */