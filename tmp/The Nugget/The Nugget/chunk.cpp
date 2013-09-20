#include "chunk.hpp"
#include "resource_finder.hpp"
#include <djah/core/randomizer.hpp>
#include <djah/core/noise.hpp>

//--------------------------------------------------------------------------------------------------
Chunk::Chunk(const math::vector2i &_coord, int _width, int _height, opengl::shader_program &_shader)
	: coordinates_(_coord)
	, width_(_width)
	, height_(_height)
	, proba_(0.0f)
	, shader_(_shader)
	, pTransformBuffer_(nullptr)
{
	initTransform();
	initMeshes();

	proba_ = utils::randomizer::random(0.0f, 10.0f);
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
Chunk::~Chunk()
{
	delete pTransformBuffer_;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Chunk::setCoordinates(const math::vector2i &_coord)
{
	if( coordinates_ != _coord )
	{
		coordinates_ = _coord;
		update();
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Chunk::setHealth(const math::vector2i &_cell, float _health)
{
	const int offset = (_cell.y * width_ + _cell.x) * transformFormat_.vertexSize();
	pTransformBuffer_->write(&_health, 1, offset);
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
float Chunk::getHealth(const math::vector2i &_cell)
{
	const int offset = (_cell.y * width_ + _cell.x) * transformFormat_.vertexSize();
	float health = 0.0f;
	pTransformBuffer_->read(&health, 1, offset);
	return health;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Chunk::initTransform()
{
	transformFormat_
		<< opengl::format::vertex_attrib<1,float>("Health", 1);

	const unsigned int vbSize = width_ * height_ * transformFormat_.vertexSize();
	pTransformBuffer_ = new opengl::vertex_buffer(vbSize, opengl::eBU_DynamicRead);

	update();
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Chunk::initMeshes()
{
	DJAH_ASSERT(pTransformBuffer_);

	pCellMesh_  = find_resource<resources::mesh>("mine_planks.mesh");
	pBlockMesh_ = find_resource<resources::mesh>("block_dirt_03.mesh");

	DJAH_ASSERT(pCellMesh_);
	DJAH_ASSERT(pBlockMesh_);

	std::for_each(pCellMesh_->subMeshes().begin(), pCellMesh_->subMeshes().end(),
	[&](resources::submesh *pSubMesh)
	{
		cellVertexArray_.addVertexBuffer(pSubMesh->pVertexBuffer, pSubMesh->vertexFormat);
	});
	cellVertexArray_.addVertexBuffer(pTransformBuffer_, transformFormat_);
	cellVertexArray_.setVertexCount(pCellMesh_->vertexCount());

	std::for_each(pBlockMesh_->subMeshes().begin(), pBlockMesh_->subMeshes().end(),
	[&](resources::submesh *pSubMesh)
	{
		blockVertexArray_.addVertexBuffer(pSubMesh->pVertexBuffer, pSubMesh->vertexFormat);
	});
	blockVertexArray_.addVertexBuffer(pTransformBuffer_, transformFormat_);
	blockVertexArray_.setVertexCount(pBlockMesh_->vertexCount());

	cellVertexArray_.init(shader_);
	blockVertexArray_.init(shader_);
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Chunk::update()
{
	unsigned int offset = 0;
	for(int i = 0; i < width_; ++i)
	{
		for(int j = 0; j < height_; ++j)
		{
			const float health =
				noise<float>::noise2
				(
					float(i+coordinates_.x*width_),
					float(j+coordinates_.y*height_)
				);

			offset += pTransformBuffer_->write(&health, 1, offset);
		}
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Chunk::draw() const
{
	shader_.sendUniform("in_ChunkProbability", proba_);
	shader_.sendUniform("in_ChunkCoord", coordinates_);
	shader_.sendUniform("in_IsBlock", false);
	cellVertexArray_.drawInstanced(width_*height_);
	shader_.sendUniform("in_IsBlock", true);
	blockVertexArray_.drawInstanced(width_*height_);
}
//--------------------------------------------------------------------------------------------------

