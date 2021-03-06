#include "game/resources/mesh.hpp"

namespace game { namespace resources {

	//----------------------------------------------------------------------------------------------
	mesh::mesh(unsigned int subMeshCount)
		: vertexCount_(0)
	{
		subMeshes_.reserve(subMeshCount);
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	mesh::~mesh()
	{
		std::for_each(subMeshes_.begin(), subMeshes_.end(),
			[](submesh *pSubMesh)
		{
			delete pSubMesh;
		});
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
    djah::u32 mesh::size() const
	{
        djah::u32 _size = 0;
		std::for_each(subMeshes_.begin(), subMeshes_.end(),
			[&](submesh *pSubMesh)
		{
			_size += pSubMesh->size();
		});
		return _size;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	void mesh::addSubMesh(submesh *pSubMesh)
	{
		subMeshes_.push_back(pSubMesh);
		vertexCount_ += pSubMesh->vertexCount;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
    void mesh::init(const djah::opengl::shader_program &sh)
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
    void mesh::addVertexBuffer(djah::opengl::vertex_buffer *pVB, const djah::opengl::vertex_format &vf)
	{
		std::for_each(subMeshes_.begin(), subMeshes_.end(),
			[&](submesh *pSubMesh)
		{
			pSubMesh->vertexArray.addVertexBuffer(pVB, vf);
		});
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
    void mesh::removeVertexBuffer(djah::opengl::vertex_buffer *pVB)
	{
		std::for_each(subMeshes_.begin(), subMeshes_.end(),
			[&](submesh *pSubMesh)
		{
			pSubMesh->vertexArray.removeVertexBuffer(pVB);
		});
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*game*/