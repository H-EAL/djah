#pragma once

#include <djah/math.hpp>
#include <djah/opengl.hpp>
#include <djah/resources/mesh.hpp>

using namespace djah;

class Chunk
{
public:
	Chunk(const math::vector2i &_coord, int _width, int _height, opengl::shader_program &_shader);
	~Chunk();

	void update();
	void draw() const;
	void setCoordinates(const math::vector2i &_coord);

private:
	void initTransform();
	void initMeshes();

private:
	math::vector2i			coordinates_;
	int						width_;
	int						height_;
	opengl::shader_program	&shader_;
	opengl::vertex_format	transformFormat_;
	opengl::vertex_buffer	*pTransformBuffer_;
	opengl::vertex_array	cellVertexArray_;
	opengl::vertex_array	blockVertexArray_;
	resources::mesh_ptr		pCellMesh_;
	resources::mesh_ptr		pBlockMesh_;
};