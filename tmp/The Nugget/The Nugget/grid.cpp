#include "grid.hpp"
#include <djah/3d/primitives.hpp>
#include <djah/core/randomizer.hpp>
#include <djah/filesystem/memory_stream.hpp>

using namespace djah;

static const int CHUNK_WIDTH = 7;
static const int CHUNK_HEIGHT = 4;
static const float CELL_SIZE = 1.18f;

//--------------------------------------------------------------------------------------------------
Grid::Grid(int _width, int _height)
	: width_(_width)
	, height_(_height)
	, shader_("chunk")
{
	initChunks();
	initFrameBuffer(1280, 800);

	pMatTex_ = d3d::texture_manager::get().find("drybed_diffuse01.jpg");
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
Grid::~Grid()
{
	for(int i = 0; i < NB_CHUNKS_W; ++i)
	{
		for(int j = 0; j < NB_CHUNKS_H; ++j)
		{
			delete pChunks_[i][j];
		}
	}

	delete pColorBuffer;
	delete pPickingBuffer;
	delete pDepthBuffer;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Grid::initChunks()
{
	for(int i = 0; i < NB_CHUNKS_W; ++i)
	{
		for(int j = 0; j < NB_CHUNKS_H; ++j)
		{
			pChunks_[i][j] = new Chunk(math::vector2i(i,j), CHUNK_WIDTH,CHUNK_HEIGHT, shader_.program());
		}
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Grid::initFrameBuffer(int resX, int resY)
{
	pColorBuffer = new opengl::texture(GL_RGB32F, resX, resY);
	pColorBuffer->bind();
	pColorBuffer->setNoFiltering();
	pColorBuffer->setPixelBuffer(GL_RGB, GL_FLOAT, nullptr);
	pColorBuffer->unbind();

	pPickingBuffer = new opengl::texture(GL_RGBA32I, resX, resY);
	pPickingBuffer->bind();
	pPickingBuffer->setNoFiltering();
	pPickingBuffer->setPixelBuffer(GL_RGBA_INTEGER, GL_INT, nullptr);
	pPickingBuffer->unbind();

	pDepthBuffer = new opengl::texture(GL_DEPTH_COMPONENT32F, resX, resY);
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
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
math::vector2i Grid::cameraPositionInChunkSpace(const math::vector3f &cameraPosition) const
{
	static const math::vector2f chunkSize(CHUNK_WIDTH * CELL_SIZE, CHUNK_HEIGHT * CELL_SIZE);
	math::vector2i cameraChunk;

	if( cameraPosition.x < 0.0f )
		cameraChunk.x = int((cameraPosition.x - chunkSize.x) / chunkSize.x);
	else
		cameraChunk.x = int(cameraPosition.x / chunkSize.x);

	if( cameraPosition.y < 0.0f )
		cameraChunk.y = int((cameraPosition.y - chunkSize.y) / chunkSize.y);
	else
		cameraChunk.y = int(cameraPosition.y / chunkSize.y);

	return cameraChunk;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Grid::draw(const math::vector3f &cameraPosition, const math::matrix4f &matVP)
{
	frameBuffer_.bindWriting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_.program().begin();
	shader_.program().sendUniform("in_VP", matVP);
	shader_.program().sendUniform("in_CellsPerChunk", math::vector2i(CHUNK_WIDTH,CHUNK_HEIGHT));
	shader_.program().sendUniform("in_CellSize", CELL_SIZE);
	shader_.program().sendUniform("in_MatSampler", 0);

	pMatTex_->bind();
	const math::vector2i &cameraChunk = cameraPositionInChunkSpace(cameraPosition);
	shader_.program().sendUniform("in_CameraChunk", cameraChunk);

	for(int i = 0; i < NB_CHUNKS_W; ++i)
	{
		for(int j = 0; j < NB_CHUNKS_H; ++j)
		{
			const math::vector2i &coord = cameraChunk + math::vector2i(i-NB_CHUNKS_W/2, j-NB_CHUNKS_H/2);
			pChunks_[i][j]->setCoordinates(coord);
			pChunks_[i][j]->draw();
		}
	}
	pMatTex_->unbind();

	shader_.program().end();
	frameBuffer_.unbindWriting();

	swapFrameBuffers();
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Grid::swapFrameBuffers()
{
	frameBuffer_.bindReading();
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0,0,1280,800, 0,0,1280,800, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glReadBuffer(GL_NONE);
	frameBuffer_.unbindReading();
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
math::vector4i Grid::cellAt(const math::vector2i &mousePos)
{
	math::vector4i pt;

	frameBuffer_.bindReading();
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glReadPixels(mousePos.x, 800-mousePos.y-1, 1, 1, GL_RGBA_INTEGER, GL_INT, pt.data);
	glReadBuffer(GL_NONE);
	frameBuffer_.unbindReading();

	pt.x -= 1;
	pt.y -= 1;

	return pt;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void Grid::destroyCell(const math::vector2i &mousePos, const math::vector3f &cameraPosition)
{
	const math::vector4i &pt = cellAt(mousePos);
	if( isCorrectCell(pt) )
	{
		const math::vector2i cell(pt.x, pt.y);
		const math::vector2i chunk(pt.z, pt.w);

		const math::vector2i &cameraChunk = cameraPositionInChunkSpace(cameraPosition);
		Chunk *pChunk = pChunks_[pt.z - cameraChunk.x + NB_CHUNKS_W/2][pt.w - cameraChunk.y+NB_CHUNKS_H/2];

		float currentHealth = pChunk->getHealth(cell);

		currentHealth = math::clamp(currentHealth - 0.1f, 0.0f, 1.0f);
		pChunk->setHealth(cell, currentHealth);
	}
}
//--------------------------------------------------------------------------------------------------

bool Grid::isCorrectCell(const djah::math::vector4i &pt) const
{
	return ( pt.x >= 0 && pt.y >= 0 );
}












std::vector< std::vector<Cell> > cells;

OldGrid::OldGrid(int width, int height)
	: width_(width)
	, height_(height)
	, shader_("lit_textured")
	, pPickingBuffer(nullptr)
	, highlightedCell_(-1,-1)
{
	const int pX = utils::randomizer::random(0,99);
	const int pY = utils::randomizer::random(0,99);
	miner_.position_.x = pX * CELL_SIZE;
	miner_.position_.y = pY * CELL_SIZE;

	pCellMesh_ = find_resource<resources::mesh>("meshes/grid_space_dirt_01.mesh");
	pBlockMesh_ = find_resource<resources::mesh>("meshes/block_dirt_01.mesh");
	pMinerMesh_ = find_resource<resources::mesh>("meshes/miner.mesh");

	cellBatcher_.init(pCellMesh_);
	blockBatcher_.init(pBlockMesh_);
	pMinerMesh_->init(shader_.program());

	cells.resize(width_);

	math::transform_f t;
	for( int i = 0; i < width_; ++i )
	{
		cells[i].resize(height_);

		for( int j = 0; j < height_; ++j )
		{
			t.setTranslation( math::vector3f(float(i)*CELL_SIZE, float(j)*CELL_SIZE, 0.0f) );
			const math::vector3f c(0.0f,0.0f,0.0f);
			cellBatcher_.add( t, c );
			u32 id = u32(-1);
			//if( utils::randomizer::random(100) < 80 )
			if( i != pX || j != pY )
			{
				cells[i][j].health = utils::randomizer::random(1, 10);
				t.setTranslation( math::vector3f(float(i)*CELL_SIZE, float(j)*CELL_SIZE, 0.2f) );
				const math::vector3f c(float(i+1), float(j+1), float(cells[i][j].health));
				id = blockBatcher_.add( t, c );
			}
			cells[i][j].id = id;
			cells[i][j].x = i;
			cells[i][j].y = j;
		}
	}

	miner_.cell_ = cells[pX][pY];

	pDiffuse_ = d3d::texture_manager::get().find("drybed_diffuse01.jpg");
	pNormal_ = d3d::texture_manager::get().find("drybed_normal01.jpg");
	pMinerTex_ = d3d::texture_manager::get().find("miner.jpg");

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

OldGrid::~OldGrid()
{
	delete pPickingBuffer;
}

void OldGrid::draw(const math::matrix4f &matVP, const math::vector3f &eyePosition)
{
	static float angle = 0.0f;
	if(highlightedCell_.x < miner_.cell_.x && highlightedCell_.y == miner_.cell_.y)
		angle = 90.0f;
	else if(highlightedCell_.x > miner_.cell_.x && highlightedCell_.y == miner_.cell_.y)
		angle = -90.0f;
	else if(highlightedCell_.x == miner_.cell_.x && highlightedCell_.y < miner_.cell_.y)
		angle = 180.0f;
	else if(highlightedCell_.x == miner_.cell_.x && highlightedCell_.y > miner_.cell_.y)
		angle = 0.0f;

	const math::matrix4f &in_World =
		math::make_scale(0.8f)
		*
		math::make_rotation(math::deg_to_rad(angle), math::vector3f::z_axis)
		*
		math::make_translation(miner_.position_)
		;

	frameBuffer_.bindWriting();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum drawBuffer[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, drawBuffer);
	cellBatcher_.program().begin();
	cellBatcher_.program().sendUniform("in_VP", matVP);
	cellBatcher_.program().sendUniform("in_Diffuse", 0);
	cellBatcher_.program().sendUniform("in_HighLight", highlightedCell_);
	pDiffuse_->bind();
	cellBatcher_.draw();
	blockBatcher_.draw();
	pDiffuse_->unbind();
	cellBatcher_.program().end();

	glDrawBuffers(1, drawBuffer);
	shader_.program().begin();
	shader_.program().sendUniform("in_World", in_World);
	shader_.program().sendUniform("in_WVP", in_World * matVP);
	shader_.program().sendUniform("in_DiffuseSampler", 0);
	pMinerTex_->bind();
	pMinerMesh_->draw();
	pMinerTex_->unbind();
	shader_.program().end();


	frameBuffer_.unbindWriting();

	frameBuffer_.bindReading();
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0,0,1280,800, 0,0,1280,800, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glReadBuffer(GL_NONE);
	frameBuffer_.unbindReading();
}

math::vector2i OldGrid::cellAt(const math::vector2i &mousePos)
{
	unsigned int data[3];
	frameBuffer_.bindReading();
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer_.id());
	glReadPixels(mousePos.x, 800-mousePos.y-1, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, data);
	glReadBuffer(GL_NONE);
	frameBuffer_.unbindReading();

	math::vector2i pt;
	pt.x = int(data[0]-1);
	pt.y = int(data[1]-1);
	return pt;
}

void OldGrid::destroyCell(const math::vector2i &mousePos)
{
	math::vector2i pt = cellAt(mousePos);
	if( isCorrectCell(pt) && highlightedCell_ == pt )
	{
		if( --(cells[pt.x][pt.y].health) == 0 )
		{
			blockBatcher_.remove(cells[pt.x][pt.y].id);
			cells[pt.x][pt.y].id = u32(-1);

			miner_.cell_ = cells[pt.x][pt.y];
			miner_.position_.x = pt.x * CELL_SIZE;
			miner_.position_.y = pt.y * CELL_SIZE;
		}
		else
		{
			blockBatcher_.set(cells[pt.x][pt.y].id, float(cells[pt.x][pt.y].health));
		}
	}
}

bool OldGrid::isCorrectCell(const djah::math::vector2i &pt) const
{
	return ( pt.x >= 0 && pt.y >= 0 && cells[pt.x][pt.y].id != u32(-1) );
}

void OldGrid::highlightCell(const djah::math::vector2i &mousePos)
{
	auto cellPos = cellAt(mousePos);
	highlightedCell_.x = -1;
	highlightedCell_.y = -1;
	if( isCorrectCell(cellPos) )
	{
		if( 
			(std::abs(cellPos.x - miner_.cell_.x) <= 1 && (cellPos.y - miner_.cell_.y) == 0)
			|| (std::abs(cellPos.y - miner_.cell_.y) <= 1 && (cellPos.x - miner_.cell_.x) == 0)
		)
		{
			highlightedCell_ = cellPos;
		}
	}
}