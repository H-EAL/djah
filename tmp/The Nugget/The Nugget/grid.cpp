#include "grid.hpp"
#include <djah/3d/primitives.hpp>
#include <djah/core/randomizer.hpp>
#include <djah/filesystem/memory_stream.hpp>

using namespace djah;

grid::grid(int width, int height)
	: width_(width)
	, height_(height)
	, shader_("lit_textured")
	, pBlockVB_(nullptr)
{
	batcher_.init<d3d::primitives::cube>();
	math::transformation_f t;
	for( int i = 0; i < width_; ++i )
	{
		for( int j = 0; j < height_; ++j )
		{
			if( utils::randomizer::random(100) < 80 )
			{
				t.setTranslation( math::vector3f(float(i), 0.0f, float(j)) );
				const math::vector3f c(utils::randomizer::random(1.0f), utils::randomizer::random(1.0f), utils::randomizer::random(1.0f));
				batcher_.add( t, c );
			}
		}
	}

	pDiffuse_ = d3d::texture_manager::get().find("drybed_diffuse01.jpg");
	pNormal_ = d3d::texture_manager::get().find("drybed_normal01.jpg");

	loadMesh();
}

grid::~grid()
{
	delete pBlockVB_;
}

void grid::loadMesh()
{
	filesystem::stream_ptr strm = filesystem::browser::get().openReadStream("meshes/grid_space_dirt_01.bdae");
	if( !strm || strm->size() <= 0 )
		return;

	filesystem::memory_stream memStrm(strm);

	pBlockVB_ = new djah::opengl::vertex_buffer(memStrm.size(), opengl::eBU_StaticDraw);
	pBlockVB_->write(memStrm.buffer(), memStrm.size());

	opengl::vertex_format vertexFormat(opengl::vertex_format::ePT_Packed);
	vertexFormat
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>();

	blockVA_.addVertexBuffer(pBlockVB_, vertexFormat);
	blockVA_.setVertexCount(474);
	blockVA_.init(shader_.program());
}

void grid::draw(const math::matrix4f &matVP)
{
	const math::matrix4f &in_World = math::make_rotation(math::deg_to_rad(90.0f), math::vector3f::x_axis)
		* math::make_scale(0.1f);
	const math::matrix4f &in_WVP   = in_World * matVP;
	pDiffuse_->bind();
	shader_.program().begin();
	shader_.program().sendUniform("in_World", in_World);
	shader_.program().sendUniform("in_WVP", in_WVP);
	shader_.program().sendUniform("in_DiffuseSampler", 0);
	//batcher_.draw(matVP);
	blockVA_.draw();
	shader_.program().end();
	pDiffuse_->unbind();
}