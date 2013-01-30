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

	void draw(const djah::math::matrix4f &matVP);

private:
	void loadMesh();

private:
	int width_;
	int height_;
	djah::d3d::primitives::batcher batcher_;
	djah::d3d::texture_ptr			pDiffuse_;
	djah::d3d::texture_ptr			pNormal_;
	djah::d3d::shader				shader_;

	djah::opengl::vertex_buffer		*pBlockVB_;
	djah::opengl::vertex_array		blockVA_;
};