#pragma once

#include <djah/math.hpp>
#include <djah/opengl.hpp>
#include <djah/3d/primitives/batcher.hpp>
#include "resource_finder.hpp"


#include "chunk.hpp"

using namespace djah;

class Grid
{
public:
	Grid(int _width, int _height);
	~Grid();

	void draw(const math::vector3f &cameraPosition, const math::matrix4f &matVP);
	math::vector4i cellAt(const math::vector2i &mousePos);
	void destroyCell(const djah::math::vector2i &mousePos, const math::vector3f &cameraPosition);

private:
	void initChunks();
	void initFrameBuffer(int resX, int resY);
	void swapFrameBuffers();
	math::vector2i cameraPositionInChunkSpace(const math::vector3f &cameraPosition) const;
	bool isCorrectCell(const djah::math::vector4i &pt) const;

private:
	int width_;
	int height_;
	d3d::shader	shader_;
	static const int NB_CHUNKS_W = 3;
	static const int NB_CHUNKS_H = 3;
	Chunk *pChunks_[NB_CHUNKS_W][NB_CHUNKS_H];

	d3d::texture_ptr pMatTex_;
	d3d::texture_ptr pWoodTex_;

	djah::opengl::frame_buffer frameBuffer_;
	djah::opengl::texture	  *pColorBuffer;
	djah::opengl::texture	  *pPickingBuffer;
	djah::opengl::texture	  *pDepthBuffer;
};











struct Cell
{
	djah::u32 id;
	djah::u32 health;
	int x, y;

	Cell() : id(0), health(0), x(-1), y(-1) {}
};

struct Miner
{
	djah::math::vector3f position_;
	Cell cell_;
};

class OldGrid
{
public:
	OldGrid(int width, int height);
	~OldGrid();

	void draw(const djah::math::matrix4f &matVP, const djah::math::vector3f &eyePosition);

	djah::math::vector2i cellAt(const djah::math::vector2i &mousePos);
	void highlightCell(const djah::math::vector2i &mousePos);
	void destroyCell(const djah::math::vector2i &mousePos);

	bool isCorrectCell(const djah::math::vector2i &pt) const;

	Miner miner_;

private:
	int width_;
	int height_;
	djah::d3d::primitives::batcher	cellBatcher_;
	djah::d3d::primitives::batcher	blockBatcher_;

	djah::d3d::texture_ptr			pDiffuse_;
	djah::d3d::texture_ptr			pNormal_;
	djah::d3d::texture_ptr			pMinerTex_;
	djah::d3d::shader				shader_;

	djah::resources::mesh_ptr		pCellMesh_;
	djah::resources::mesh_ptr		pBlockMesh_;
	djah::resources::mesh_ptr		pMinerMesh_;

	djah::opengl::frame_buffer		frameBuffer_;
	djah::opengl::texture			*pColorBuffer;
	djah::opengl::texture			*pPickingBuffer;
	djah::opengl::texture			*pDepthBuffer;

	djah::math::vector2i			highlightedCell_;
};