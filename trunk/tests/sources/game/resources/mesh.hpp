#ifndef GAME_RESOURCES_MESH_HPP
#define GAME_RESOURCES_MESH_HPP

#include <vector>
#include <memory>
#include "djah/types.hpp"
#include "djah/resources/asset.hpp"
#include "djah/opengl.hpp"

namespace game { namespace resources {

	//----------------------------------------------------------------------------------------------
	struct submesh
	{
		submesh()
			: vertexCount(0)
            , vertexFormat(djah::opengl::vertex_format::ePT_Packed)
			, pVertexBuffer(nullptr)
		{
		}

		~submesh()
		{
			delete pVertexBuffer;
		}

		djah::u32 size() const { return vertexCount * vertexFormat.vertexSize(); }

        djah::u32                   vertexCount;
        djah::opengl::vertex_format	vertexFormat;
        djah::opengl::vertex_array	vertexArray;
        djah::opengl::vertex_buffer	*pVertexBuffer;
	};
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class mesh
		: public djah::resources::asset
	{
	public:
		mesh(unsigned int subMeshCount);
		virtual ~mesh();

        virtual djah::u32 size() const override;

		void addSubMesh(submesh *pSubMesh);
		void draw() const;
		void drawInstanced(int instanceCount) const;
        void init(const djah::opengl::shader_program &sh);
        void addVertexBuffer(djah::opengl::vertex_buffer *pVB, const djah::opengl::vertex_format &vf);
        void removeVertexBuffer(djah::opengl::vertex_buffer *pVB);
		const std::vector<submesh*>& subMeshes() const { return subMeshes_; }
		unsigned int vertexCount() const { return vertexCount_; }

	private:
		unsigned int vertexCount_;
		std::vector<submesh*> subMeshes_;
	};
	//----------------------------------------------------------------------------------------------

	typedef std::shared_ptr<mesh> mesh_sptr;

} /*resources*/ } /*game*/

#endif /* GAME_RESOURCES_MESH_HPP */