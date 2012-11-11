#include "bump_mapping_test.hpp"
#include "djah/types.hpp"
#include "resource_finder.hpp"
#include "djah/video/primitives/sphere.hpp"
#include "djah/video/primitives/cube.hpp"
#include "djah/resources/media_manager.hpp"
#include "djah/filesystem/browser.hpp"
#include "djah/filesystem/memory_stream.hpp"

using namespace djah;
using namespace video;

//--------------------------------------------------------------------------------------------------
BumpMappingTest::BumpMappingTest(djah::system::device_ptr pDevice, const djah::system::input::gamepad &g, Camera &cam)
	: test_base(pDevice)
	, cam_(cam)
	, gamepad_(g)
	, pVB_(nullptr)
	, pVA_(nullptr)
	, pDiffuse_(nullptr)
	, pNormalMap_(nullptr)
	, pPlaneVB_(nullptr)
	, pPlaneVA_(nullptr)
	, pPlaneDiffuse_(nullptr)
	, pPlaneNormalMap_(nullptr)
	, shader_("bump_mapping")
{
	const float w = static_cast<float>(pDevice_->videoConfig().width);
	const float h = static_cast<float>(pDevice_->videoConfig().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);

	opengl::vertex_shader vertexShader( loadShaderSource("shaders/bump_mapping.vert") );
	opengl::pixel_shader  pixelShader(  loadShaderSource("shaders/bump_mapping.frag") );
	if( vertexShader.compile() && pixelShader.compile() )
	{
		shader_.attach(vertexShader);
		shader_.attach(pixelShader);
		shader_.link();
	}

	// CTHULHU
	filesystem::memory_stream strm( filesystem::browser::get().openReadStream("3d/cthulhu.bdae") );
	
	pVB_ = new opengl::vertex_buffer(strm.size(), opengl::eBU_StaticDraw);
	pVB_->write(strm.buffer(), strm.size());

	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>()
		<< opengl::format::vertex_attrib<3,float>("Tangent");

	pVA_ = new opengl::vertex_array(vertexFormat, pVB_);
	pVA_->init(shader_);

	resources::image_ptr diffuseImg = find_resource<resources::image>("textures/cthulhu.jpg");
	if( diffuseImg )
	{
		pDiffuse_ = new opengl::texture(GL_RGB, diffuseImg->width(), diffuseImg->height());
		pDiffuse_->bind();
		pDiffuse_->setBestFiltering();
		GLenum pixelFormat = diffuseImg->channels() == 1 ? GL_LUMINANCE : GL_BGR;			
		pDiffuse_->setPixelBuffer(pixelFormat, GL_UNSIGNED_BYTE, diffuseImg->pixels());
	}

	resources::image_ptr nmapImg = find_resource<resources::image>("textures/cthulhu_normalmap.jpg");
	if( nmapImg )
	{
		pNormalMap_ = new opengl::texture(GL_RGB, nmapImg->width(), nmapImg->height());
		pNormalMap_->bind();
		pNormalMap_->setBestFiltering();
		GLenum pixelFormat = nmapImg->channels() == 1 ? GL_LUMINANCE : GL_BGR;			
		pNormalMap_->setPixelBuffer(pixelFormat, GL_UNSIGNED_BYTE, nmapImg->pixels());
	}

	// PLANE
	const float w2 = 3.0f;
	const float h2 = 6.0f;
	const float planeVertices[] =
	{
		-w2,0.0f,-3.0f,  0.0f,0.0f,1.0f,  0.0f,0.0f,  1.0f,0.0f,0.0f, 
		-w2,  h2,-3.0f,  0.0f,0.0f,1.0f,  0.0f,1.0f,  1.0f,0.0f,0.0f,
		 w2,  h2,-3.0f,  0.0f,0.0f,1.0f,  1.0f,1.0f,  1.0f,0.0f,0.0f,
		 w2,0.0f,-3.0f,  0.0f,0.0f,1.0f,  1.0f,0.0f,  1.0f,0.0f,0.0f,
	};

	const u8 planeIndices[] =
	{
		0,3,2, 0,2,1
	};

	pPlaneIB_ = new opengl::index_buffer(sizeof(planeIndices), opengl::eBU_StaticDraw);
	pPlaneIB_->write(planeIndices);

	pPlaneVB_ = new opengl::vertex_buffer(sizeof(planeVertices), opengl::eBU_StaticDraw);
	pPlaneVB_->write(planeVertices);

	pPlaneVA_ = new opengl::vertex_array(vertexFormat, pPlaneVB_, pPlaneIB_);
	pPlaneVA_->init(shader_);

	resources::image_ptr planeDiffuseImg = find_resource<resources::image>("textures/color_map.jpg");
	if( planeDiffuseImg )
	{
		pPlaneDiffuse_ = new opengl::texture(GL_RGB, planeDiffuseImg->width(), planeDiffuseImg->height(), true);
		pPlaneDiffuse_->bind();
		pPlaneDiffuse_->setBestFiltering();
		GLenum pixelFormat = planeDiffuseImg->channels() == 1 ? GL_LUMINANCE : GL_BGR;			
		pPlaneDiffuse_->setPixelBuffer(pixelFormat, GL_UNSIGNED_BYTE, planeDiffuseImg->pixels());
	}

	resources::image_ptr planeNMapImg = find_resource<resources::image>("textures/normal_map.jpg");
	if( planeNMapImg )
	{
		pPlaneNormalMap_ = new opengl::texture(GL_RGB, planeNMapImg->width(), planeNMapImg->height(), true);
		pPlaneNormalMap_->bind();
		pPlaneNormalMap_->setBestFiltering();
		GLenum pixelFormat = planeNMapImg->channels() == 1 ? GL_LUMINANCE : GL_BGR;			
		pPlaneNormalMap_->setPixelBuffer(pixelFormat, GL_UNSIGNED_BYTE, planeNMapImg->pixels());
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
BumpMappingTest::~BumpMappingTest()
{
	delete pVB_;
	delete pVA_;
	delete pDiffuse_;
	delete pNormalMap_;

	delete pPlaneVB_;
	delete pPlaneVA_;
	delete pPlaneDiffuse_;
	delete pPlaneNormalMap_;
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
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	const math::matrix4f &matVP = math::make_look_at(cam_.eye(), cam_.center(), cam_.up()) * matPerspectiveProj_;

	opengl::frame_buffer::unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_.begin();
	shader_.sendUniform( "in_DiffuseSampler", 0);
	shader_.sendUniform( "in_NormalMapSampler", 1);

	// Cthulhu
	static const math::matrix4f cthulhuMatWorld = math::make_rotation(-90.0f, math::vector3f::y_axis) * math::make_translation(10.0f,0.0f,0.0f);
	opengl::texture::set_active_unit(0);
	pDiffuse_->bind();
	opengl::texture::set_active_unit(1);
	pNormalMap_->bind();

	shader_.sendUniformMatrix( "in_World", cthulhuMatWorld);
	shader_.sendUniformMatrix( "in_WVP", cthulhuMatWorld*matVP);
	shader_.sendUniform( "in_UseNMap", useNMap);
	pVA_->draw();

	// Plane
	opengl::texture::set_active_unit(0);
	pPlaneDiffuse_->bind();
	opengl::texture::set_active_unit(1);
	pPlaneNormalMap_->bind();

	static const math::matrix4f planeMatWorld = math::matrix4f::identity;
	shader_.sendUniformMatrix( "in_World", planeMatWorld);
	shader_.sendUniformMatrix( "in_WVP", matVP);
	pPlaneVA_->draw();

	opengl::texture::unbind();
	opengl::texture::set_active_unit(0);
	opengl::texture::unbind();

	shader_.end();

	glDisable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------------------------------------------