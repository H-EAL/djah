#ifndef DJAH_RESOURCES_MESH_HPP
#define DJAH_RESOURCES_MESH_HPP

#include <vector>
#include <memory>
#include "djah/types.hpp"
#include "djah/resources/asset.hpp"
#include "djah/opengl.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	struct submesh
	{
		submesh()
			: vertexCount(0)
			, vertexFormat(opengl::vertex_format::ePT_Packed)
			, pVertexBuffer(nullptr)
		{
		}

		~submesh()
		{
			delete pVertexBuffer;
		}

		u32 size() const { return vertexCount * vertexFormat.vertexSize(); }

		unsigned int			vertexCount;
		opengl::vertex_format	vertexFormat;
		opengl::vertex_array	vertexArray;
		opengl::vertex_buffer	*pVertexBuffer;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class mesh
		: public asset
	{
	public:
		mesh(unsigned int subMeshCount);
		virtual ~mesh();

		virtual u32 size() const;

		void addSubMesh(submesh *pSubMesh);
		void draw() const;
		void drawInstanced(int instanceCount) const;
		void init(const opengl::shader_program &sh);
		void addVertexBuffer(opengl::vertex_buffer *pVB, const opengl::vertex_format &vf);
		void removeVertexBuffer(opengl::vertex_buffer *pVB);
		const std::vector<submesh*>& subMeshes() const { return subMeshes_; }
		unsigned int vertexCount() const { return vertexCount_; }

	private:
		unsigned int vertexCount_;
		std::vector<submesh*> subMeshes_;
	};
	//----------------------------------------------------------------------------------------------

	typedef std::shared_ptr<mesh> mesh_sptr;

} /*resources*/ } /*djah*/

#endif /* DJAH_RESOURCES_MESH_HPP */