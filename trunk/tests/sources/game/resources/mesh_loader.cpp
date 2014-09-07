#include "game/resources/mesh_loader.hpp"

#include "djah/debug/assertion.hpp"
#include "djah/filesystem/stream.hpp"


namespace game { namespace resources {

	//----------------------------------------------------------------------------------------------
    bool mesh_loader::loadFromStream(djah::filesystem::stream &strm, const std::string &fileName, mesh_sptr &spMesh)
	{
		bool success = false;

		unsigned int version = 0;
		strm >> version;

		if( version == 0x00010000 )
		{
			unsigned int subMeshCount = 0;
			strm >> subMeshCount;

			if( ensure(subMeshCount > 0) )
			{
				spMesh.reset( new mesh(subMeshCount) );

				for(unsigned int i = 0; i < subMeshCount; ++i)
				{
					submesh *pSubMesh = loadSubMesh(strm);
					check(pSubMesh);

					spMesh->addSubMesh(pSubMesh);
				}

				success = true;
			}
		}

		return success;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
    submesh* mesh_loader::loadSubMesh(djah::filesystem::stream &strm)
	{
		submesh *pSubMesh = new submesh;

		strm >> pSubMesh->vertexCount;
		check(pSubMesh->vertexCount > 0);

		unsigned int attributeCount = 0;
		strm >> attributeCount;
		check(attributeCount > 0);

		for(unsigned int i = 0; i < attributeCount; ++i)
		{
			unsigned int nameSize = 0;
			strm >> nameSize;
			std::string attributeName;
			attributeName.resize(nameSize);
			strm.read(&attributeName[0], nameSize);

			unsigned int size = 0;
			strm >> size;
			unsigned int count = 0;
			strm >> count;
			unsigned int valueType = 0;
			strm >> valueType;
			unsigned int divisor = 0;
			strm >> divisor;

			pSubMesh->vertexFormat
                << djah::opengl::format::vertex_attrib_base(attributeName, size, count, valueType, divisor);
		}

		const unsigned int vertexBufferSize = pSubMesh->vertexCount * pSubMesh->vertexFormat.vertexSize();

        std::unique_ptr<djah::byte[]> data(new djah::byte[vertexBufferSize]);
		const auto bytesRead = strm.read(data.get(), vertexBufferSize);
		check(bytesRead == vertexBufferSize);

        pSubMesh->pVertexBuffer = new djah::opengl::vertex_buffer(vertexBufferSize, djah::opengl::eBU_StaticDraw);
		pSubMesh->pVertexBuffer->write(data.get(), vertexBufferSize);
		pSubMesh->vertexArray.addVertexBuffer(pSubMesh->pVertexBuffer, pSubMesh->vertexFormat);

		pSubMesh->vertexArray.setVertexCount(pSubMesh->vertexCount);

		return pSubMesh;
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*game*/