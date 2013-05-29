#include "bump_mapping_test.hpp"
#include "djah/types.hpp"
#include "resource_finder.hpp"
#include "djah/3d/primitives.hpp"
#include "djah/resources/media_manager.hpp"
#include "djah/filesystem/browser.hpp"
#include "djah/filesystem/memory_stream.hpp"

using namespace djah;
using namespace d3d;

//--------------------------------------------------------------------------------------------------
BumpMappingTest::BumpMappingTest(djah::system::device_sptr pDevice, const djah::system::input::gamepad &g, Camera &cam)
	: test_base(pDevice)
	, cam_(cam)
	, gamepad_(g)
	, pVB_(nullptr)
	, pDiffuse_(nullptr)
	, pNormalMap_(nullptr)
	, pPlaneVB_(nullptr)
	, pPlaneDiffuse_(nullptr)
	, pPlaneNormalMap_(nullptr)
	, shader_("bump_mapping")
{
	const float w = static_cast<float>(pDevice_->config().width);
	const float h = static_cast<float>(pDevice_->config().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);

	// CTHULHU
	filesystem::memory_stream strm( filesystem::browser::get().openReadStream("3d/cthulhu_bm.bdae") );
	
	pVB_ = new opengl::vertex_buffer(strm.size(), opengl::eBU_StaticDraw);
	pVB_->write(strm.buffer(), strm.size());

	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>()
		<< opengl::format::vertex_attrib<3,float>("Tangent")
		<< opengl::format::vertex_attrib<3,float>("Binormal");

	va_.addVertexBuffer(pVB_, vertexFormat);
	va_.setVertexCount(pVB_->size() / vertexFormat.vertexSize());
	va_.init(shader_.program());

	pDiffuse_   = d3d::texture_manager::get().find("cthulhu.jpg");
	pNormalMap_ = d3d::texture_manager::get().find("cthulhu_normalmap.jpg");


	// PLANE
	const float w2 = 3.0f;
	const float h2 = 6.0f;
	const float planeVertices[] =
	{
		-w2,0.0f,-3.0f,  0.0f,0.0f,1.0f,  0.0f,0.0f,  1.0f,0.0f,0.0f,  0.0f,1.0f,0.0f,
		-w2,  h2,-3.0f,  0.0f,0.0f,1.0f,  0.0f,1.0f,  1.0f,0.0f,0.0f,  0.0f,1.0f,0.0f,
		 w2,  h2,-3.0f,  0.0f,0.0f,1.0f,  1.0f,1.0f,  1.0f,0.0f,0.0f,  0.0f,1.0f,0.0f,
		 w2,0.0f,-3.0f,  0.0f,0.0f,1.0f,  1.0f,0.0f,  1.0f,0.0f,0.0f,  0.0f,1.0f,0.0f,
	};

	const u8 planeIndices[] =
	{
		0,3,2, 0,2,1
	};

	pPlaneIB_ = new opengl::index_buffer(sizeof(planeIndices), opengl::eBU_StaticDraw);
	pPlaneIB_->write(planeIndices);

	pPlaneVB_ = new opengl::vertex_buffer(sizeof(planeVertices), opengl::eBU_StaticDraw);
	pPlaneVB_->write(planeVertices);

	planeVA_.addVertexBuffer(pPlaneVB_, vertexFormat);
	planeVA_.setIndexBuffer(pPlaneIB_);
	planeVA_.setVertexCount(2*3);
	planeVA_.init(shader_.program());

	pPlaneDiffuse_   = d3d::texture_manager::get().find("color_map.jpg");
	pPlaneNormalMap_ = d3d::texture_manager::get().find("normal_map.jpg");
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
BumpMappingTest::~BumpMappingTest()
{
	delete pVB_;

	delete pPlaneVB_;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void BumpMappingTest::onInit()
{
	opengl::frame_buffer::bind_default_frame_buffer();
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void BumpMappingTest::onExit()
{
	glDisable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------------------------------------------

static bool useNMap = true;
//--------------------------------------------------------------------------------------------------
void BumpMappingTest::update(float dt)
{
	if( gamepad_.getButton(system::input::eX360_A).pressed() )
		useNMap = !useNMap;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void BumpMappingTest::draw()
{
	const math::matrix4f &matVP = math::make_look_at(cam_.eye(), cam_.center(), cam_.up()) * matPerspectiveProj_;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_.program().begin();
	shader_.program().sendUniform( "in_DiffuseSampler", 0);
	shader_.program().sendUniform( "in_NormalMapSampler", 1);

	// Cthulhu
	static const math::matrix4f cthulhuMatWorld = math::make_rotation(-90.0f, math::vector3f::y_axis) * math::make_translation(10.0f,0.0f,0.0f);
	opengl::texture::set_active_unit(0);
	pDiffuse_->bind();
	opengl::texture::set_active_unit(1);
	pNormalMap_->bind();

	shader_.program().sendUniform( "in_World", cthulhuMatWorld);
	shader_.program().sendUniform( "in_WVP", cthulhuMatWorld*matVP);
	shader_.program().sendUniform( "in_UseNMap", useNMap);
	va_.draw();

	// Plane
	opengl::texture::set_active_unit(0);
	pPlaneDiffuse_->bind();
	opengl::texture::set_active_unit(1);
	pPlaneNormalMap_->bind();

	static const math::matrix4f planeMatWorld = math::matrix4f::identity;
	shader_.program().sendUniform( "in_World", planeMatWorld);
	shader_.program().sendUniform( "in_WVP", matVP);
	planeVA_.draw();

	opengl::texture::unbind();
	opengl::texture::set_active_unit(0);
	opengl::texture::unbind();

	shader_.program().end();
}
//--------------------------------------------------------------------------------------------------