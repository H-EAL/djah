#include "grid.hpp"
#include <djah/3d/primitives.hpp>
#include <djah/core/randomizer.hpp>
#include <djah/filesystem/memory_stream.hpp>

using namespace djah;

struct CELL
{
	CELL() : id(0), health(0) {}

	u32 id;
	u32 health;
};
std::vector< std::vector<CELL> > cells;

grid::grid(int width, int height)
	: width_(width)
	, height_(height)
	, shader_("lit_textured")
	, pPickingBuffer(nullptr)
	, highlightedCell_(-1,-1)
{
	pCellMesh_ = find_resource<resources::mesh>("meshes/grid_space_dirt_01.mesh");
	pBlockMesh_ = find_resource<resources::mesh>("meshes/block_dirt_01.mesh");

	cellBatcher_.init(pCellMesh_);
	blockBatcher_.init(pBlockMesh_);

	const float s = 1.57f;
	cells.resize(width_);

	math::transformation_f t;
	for( int i = 0; i < width_; ++i )
	{
		cells[i].resize(height_);

		for( int j = 0; j < height_; ++j )
		{
			t.setTranslation( math::vector3f(float(i)*s, 0.0f, float(j)*s) );
			const math::vector3f c(0.0f,0.0f,0.0f);
			cellBatcher_.add( t, c );
			u32 id = u32(-1);
			if( utils::randomizer::random(100) < 80 )
			{
				cells[i][j].health = utils::randomizer::random(1, 10);
				t.setTranslation( math::vector3f(float(i)*s, 0.2f, float(j)*s) );
				const math::vector3f c(float(i+1), float(j+1), float(cells[i][j].health));
				id = blockBatcher_.add( t, c );
			}
			cells[i][j].id = id;
		}
	}

	pDiffuse_ = d3d::texture_manager::get().find("drybed_diffuse01.jpg");
	pNormal_ = d3d::texture_manager::get().find("drybed_normal01.jpg");

	pColorBuffer = new opengl::texture(GL_RGB32F, 1280, 800);
	pColorBuffer->bind();
	pColorBuffer->setNoFiltering();
	pColorBuffer->setPixelBuffer(GL_RGB, GL_FLOAT, nullptr);
	pColorBuffer->unbind();

	pPickingBuffer = new opengl::texture(GL_RGB32UI, 1280, 800);
	pPickingBuffer->bind();
	pPickingBuffer->setNoFiltering();
	pPickingBuffer->setPixelBuffer(GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
	pPickingBuffer->unbind();

	pDepthBuffer = new opengl::texture(GL_DEPTH_COMPONENT32F, 1280, 800);
	pDepthBuffer->bind();
	pDepthBuffer->setNoFiltering();
	pDepthBuffer->setPixelBuffer(GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	pDepthBuffer->unbind();

	frameBuffer_.bindWriting();
	GLenum drawBuffer[2];
	drawBuffer[0] = (GLenum)frameBuffer_.attach(*pColorBuffer);
	drawBuffer[1] = (GLenum)frameBuffer_.attach(*pPickingBuffer);
	frameBuffer_.attach(*pDepthBuffer);

	glDrawBuffers(2, drawBuffer);
	DJAH_ASSERT(frameBuffer_.isComplete());
	frameBuffer_.unbindWriting();
		
}

grid::~grid()
{
	delete pPickingBuffer;
}

void grid::draw(const math::matrix4f &matVP, const math::vector3f &eyePosition)
{
	const math::matrix4f &in_World = math::matrix4f::identity;

	frameBuffer_.bindWriting();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pDiffuse_->bind();

	cellBatcher_.program().begin();
	cellBatcher_.program().sendUniform("in_VP", matVP);
	cellBatcher_.program().sendUniform("in_Diffuse", 0);
	cellBatcher_.program().sendUniform("in_HighLight", highlightedCell_);
	cellBatcher_.draw();
	blockBatcher_.draw();
	cellBatcher_.program().end();

	pDiffuse_->unbind();
	frameBuffer_.unbindWriting();

	frameBuffer_.bindReading();
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0,0,1280,800, 0,0,1280,800, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glReadBuffer(GL_NONE);
	frameBuffer_.unbindReading();
}

math::vector2i grid::cellAt(const math::vector2i &mousePos)
{
	unsigned int data[3];
	frameBuffer_.bindReading();
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer_.id());
	glReadPixels(mousePos.x, 800-mousePos.y-1, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, data);
	glReadBuffer(GL_NONE);
	frameBuffer_.unbindReading();

	math::vector2i pt;
	pt.x = int(-data[0]-1);
	pt.y = int(-data[1]-1);
	return pt;
}

void grid::destroyCell(const math::vector2i &mousePos)
{
	math::vector2i pt = cellAt(mousePos);
	if( isCorrectCell(pt) )
	{
		if( --(cells[pt.x][pt.y].health) == 0 )
		{
			blockBatcher_.remove(cells[pt.x][pt.y].id);
			cells[pt.x][pt.y].id = u32(-1);
		}
		else
		{
			blockBatcher_.set(cells[pt.x][pt.y].id, float(cells[pt.x][pt.y].health));
		}
	}
}

bool grid::isCorrectCell(const djah::math::vector2i &pt) const
{
	return ( pt.x >= 0 && pt.y >= 0 && cells[pt.x][pt.y].id != u32(-1) );
}

void grid::highlightCell(const djah::math::vector2i &mousePos)
{
	highlightedCell_ = cellAt(mousePos);
}