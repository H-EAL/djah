#include "basic_test.hpp"
#include "djah/types.hpp"
#include "resource_finder.hpp"
#include "djah/video/primitives/sphere.hpp"
#include "djah/video/primitives/cube.hpp"
#include "djah/resources/media_manager.hpp"

using namespace djah;
using namespace video;

//--------------------------------------------------------------------------------------------------
BasicTest::BasicTest(djah::system::device_ptr pDevice, Camera &cam)
	: test_base(pDevice)
	, cam_(cam)
	, pCubeVB_(nullptr)
	, pCubeVA_(nullptr)
	, pCubeTexture_(nullptr)
	, pSphereVB_(nullptr)
	, pSphereVA_(nullptr)
	, pSphereTexture_(nullptr)
	, cubeShader_("textured_cube")
	, sphereShader_("textured_sphere")
{
	const float w = static_cast<float>(pDevice_->videoConfig().width);
	const float h = static_cast<float>(pDevice_->videoConfig().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);

	//initCube();
	//initSphere();
	initPrimitive<primitives::cube>
	(
		"shaders/textured_cube.vert",
		"shaders/textured_cube.frag",
		cubeShader_,
		pCubeVB_,
		pCubeVA_,
		"textures/wood.jpg",
		pCubeTexture_
	);

	initPrimitive<primitives::sphere>
	(
		"shaders/textured_sphere.vert",
		"shaders/textured_sphere.frag",
		sphereShader_,
		pSphereVB_,
		pSphereVA_,
		"textures/earth.jpg",
		pSphereTexture_
	);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
template<typename P>
void BasicTest::initPrimitive(
	const std::string &vsFile,
	const std::string &psFile,
	djah::opengl::shader_program &sp,
	djah::opengl::vertex_buffer *&pVB,
	djah::opengl::vertex_array *&pVA,
	const std::string &textureFile,
	djah::opengl::texture *&pTex)
{
	opengl::vertex_shader vertexShader( loadShaderSource(vsFile) );
	opengl::pixel_shader  pixelShader(  loadShaderSource(psFile) );

	if( vertexShader.compile() && pixelShader.compile() )
	{
		sp.attach(vertexShader);
		sp.attach(pixelShader);
		sp.link();
	}

	P p;
	const std::vector<geometry::triangle3f> &triangles = p.construct<3>();

	pVB = new opengl::vertex_buffer(triangles.size() * sizeof(geometry::triangle3f), opengl::eBU_StaticDraw);
	pVB->write(&triangles[0], triangles.size());

	opengl::vertex_format vertexFormat;
	vertexFormat.record() << opengl::format::position<3,float>();
	pVA = new opengl::vertex_array(vertexFormat, pVB);
	pVA->init(sp);

	resources::image_ptr img = find_resource<resources::image>(textureFile);
	if( img )
	{
		pTex = new opengl::texture(GL_RGB, img->width(), img->height());
		pTex->bind();
		pTex->setBestFiltering();
		pTex->setPixelBuffer(GL_BGR, GL_UNSIGNED_BYTE, img->pixels());
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
BasicTest::~BasicTest()
{
	delete pCubeVB_;
	delete pCubeVA_;
	delete pCubeTexture_;

	delete pSphereVB_;
	delete pSphereVA_;
	delete pSphereTexture_;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void BasicTest::update(float dt)
{
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void BasicTest::draw()
{
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	opengl::frame_buffer::unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const math::matrix4f &matVP = math::make_look_at(cam_.eye(), cam_.center(), cam_.up()) * matPerspectiveProj_;

	const math::matrix4f &matSphereWorld = math::make_scale(1.0f) * math::make_translation(1.0f,0.0f,0.0f);
	sphereShader_.begin();
	sphereShader_.sendUniformMatrix( "in_WVP", matSphereWorld * matVP);
	pSphereTexture_->bind();
	pSphereVA_->draw();
	pSphereTexture_->unbind();
	sphereShader_.end();

	static const math::matrix4f &matCubeWorld = math::make_scale(1.0f) * math::make_translation(-1.0f,0.0f,0.0f);
	cubeShader_.begin();
	cubeShader_.sendUniformMatrix( "in_WVP", matCubeWorld * matVP);
	pCubeTexture_->bind();
	pCubeVA_->draw();
	pCubeTexture_->unbind();
	cubeShader_.end();

	glDisable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------------------------------------------