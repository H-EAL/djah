#include "djah/resources/mesh.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	mesh::mesh(unsigned int subMeshCount)
	{
		subMeshes_.reserve(subMeshCount);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	mesh::~mesh()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void mesh::addSubMesh(submesh *pSubMesh)
	{
		subMeshes_.push_back(pSubMesh);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void mesh::init(const opengl::shader_program &sh)
	{
		std::for_each(subMeshes_.begin(), subMeshes_.end(),
			[&](submesh *pSubMesh)
		{
			pSubMesh->vertexArray.init(sh);
		});
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void mesh::draw() const
	{
		std::for_each(subMeshes_.begin(), subMeshes_.end(),
		[](submesh *pSubMesh)
		{
			pSubMesh->vertexArray.draw();
		});
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void mesh::drawInstanced(int instanceCount) const
	{
		std::for_each(subMeshes_.begin(), subMeshes_.end(),
		[&](submesh *pSubMesh)
		{
			pSubMesh->vertexArray.drawInstanced(instanceCount);
		});
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void mesh::addVertexBuffer(opengl::vertex_buffer *pVB, const opengl::vertex_format &vf)
	{
		std::for_each(subMeshes_.begin(), subMeshes_.end(),
			[&](submesh *pSubMesh)
		{
			pSubMesh->vertexArray.addVertexBuffer(pVB, vf);
		});
	}
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	void mesh::removeVertexBuffer(opengl::vertex_buffer *pVB)
	{
		std::for_each(subMeshes_.begin(), subMeshes_.end(),
			[&](submesh *pSubMesh)
		{
			pSubMesh->vertexArray.removeVertexBuffer(pVB);
		});
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/