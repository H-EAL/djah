#include "djah/resources/mesh_loader.hpp"
#include "djah/filesystem/stream.hpp"
#include "djah/debug/assertion.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	mesh_loader::mesh_loader()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	mesh_loader::~mesh_loader()
	{
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	mesh* mesh_loader::loadFromStream(filesystem::stream &strm, const std::string &fileName)
	{
		mesh *pMesh = nullptr;

		unsigned int version = 0;
		strm >> version;

		if( version == 0x00010000 )
		{
			unsigned int subMeshCount = 0;
			strm >> subMeshCount;

			DJAH_ASSERT(subMeshCount > 0);
			pMesh = new mesh(subMeshCount);

			for(unsigned int i = 0; i < subMeshCount; ++i)
			{
				submesh *pSubMesh = loadSubMesh(strm);
				DJAH_ASSERT(pSubMesh);

				pMesh->addSubMesh(pSubMesh);
			}
		}

		return pMesh;
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	submesh* mesh_loader::loadSubMesh(filesystem::stream &strm)
	{
		submesh *pSubMesh = new submesh;

		strm >> pSubMesh->vertexCount;
		DJAH_ASSERT(pSubMesh->vertexCount > 0);

		unsigned int attributeCount = 0;
		strm >> attributeCount;
		DJAH_ASSERT(attributeCount > 0);

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
				<< opengl::format::vertex_attrib_base(attributeName, size, count, valueType, divisor);
		}

		const unsigned int vertexBufferSize = pSubMesh->vertexCount * pSubMesh->vertexFormat.vertexSize();

		std::unique_ptr<byte[]> data( new byte[vertexBufferSize] );
		strm.read(data.get(), vertexBufferSize);

		pSubMesh->pVertexBuffer = new opengl::vertex_buffer(vertexBufferSize, opengl::eBU_StaticDraw);
		pSubMesh->pVertexBuffer->write(data.get(), vertexBufferSize);
		pSubMesh->vertexArray.addVertexBuffer(pSubMesh->pVertexBuffer, pSubMesh->vertexFormat);

		pSubMesh->vertexArray.setVertexCount(pSubMesh->vertexCount);

		return pSubMesh;
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/