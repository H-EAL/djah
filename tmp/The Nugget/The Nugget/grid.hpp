#pragma once

#include <djah/math.hpp>
#include <djah/opengl.hpp>
#include <djah/3d/primitives/batcher.hpp>
#include "resource_finder.hpp"

class grid
{
public:
	grid(int width, int height);
	~grid();

	void draw(const djah::math::matrix4f &matVP, const djah::math::vector3f &eyePosition);

	djah::math::vector2i cellAt(const djah::math::vector2i &mousePos);
	void highlightCell(const djah::math::vector2i &mousePos);
	void destroyCell(const djah::math::vector2i &mousePos);

	bool isCorrectCell(const djah::math::vector2i &pt) const;

private:
	int width_;
	int height_;
	djah::d3d::primitives::batcher	cellBatcher_;
	djah::d3d::primitives::batcher	blockBatcher_;

	djah::d3d::texture_ptr			pDiffuse_;
	djah::d3d::texture_ptr			pNormal_;
	djah::d3d::shader				shader_;

	djah::resources::mesh_ptr		pCellMesh_;
	djah::resources::mesh_ptr		pBlockMesh_;

	djah::opengl::frame_buffer		frameBuffer_;
	djah::opengl::texture			*pColorBuffer;
	djah::opengl::texture			*pPickingBuffer;
	djah::opengl::texture			*pDepthBuffer;

	djah::math::vector2i			highlightedCell_;
};