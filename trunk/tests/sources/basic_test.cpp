#include "basic_test.hpp"
#include "djah/types.hpp"
#include "resource_finder.hpp"
#include "djah/3d/primitives.hpp"
#include "djah/resources/media_manager.hpp"

using namespace djah;
using namespace d3d;

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
	, pCylinderVB_(nullptr)
	, pCylinderVA_(nullptr)
	, pCylinderTexture_(nullptr)
	, pConeVB_(nullptr)
	, pConeVA_(nullptr)
	, pConeTexture_(nullptr)
	, shader_("lit_textured")
	, shaderColored_("uniform_color")
{
	const float w = static_cast<float>(pDevice_->videoConfig().width);
	const float h = static_cast<float>(pDevice_->videoConfig().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);

	initPrimitive<primitives::cube>
	(
		pCubeVB_,
		pCubeVA_,
		"textures/debug1.jpg",
		pCubeTexture_
	);
	
	initPrimitive<primitives::iso_sphere>
	(
		pSphereVB_,
		pSphereVA_,
		"textures/earth.jpg",
		pSphereTexture_,
		4
	);

	initPrimitive<primitives::cylinder>
	(
		pCylinderVB_,
		pCylinderVA_,
		"textures/earth.jpg",
		pCylinderTexture_,
		4
	);

	initPrimitive<primitives::cone>
	(
		pConeVB_,
		pConeVA_,
		"textures/earth.jpg",
		pConeTexture_,
		4
	);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
template<typename P>
void BasicTest::initPrimitive(
	djah::opengl::vertex_buffer *&pVB,
	djah::opengl::vertex_array *&pVA,
	const std::string &textureFile,
	djah::opengl::texture *&pTex,
	int level)
{
	const std::vector<primitives::triangle> &triangles = P().construct(level);

	pVB = new opengl::vertex_buffer(triangles.size() * sizeof(primitives::triangle), opengl::eBU_StaticDraw);
	pVB->write(&triangles[0], triangles.size());

	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>();

	pVA = new opengl::vertex_array(vertexFormat, pVB);
	pVA->init(shader_.program());

	resources::image_ptr img = find_resource<resources::image>(textureFile);
	if( img )
	{
		pTex = new opengl::texture(GL_RGB, img->width(), img->height(), true);
		pTex->bind();
		pTex->setBestFiltering();
		pTex->setWrapMode(GL_REPEAT);
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

	delete pCylinderVB_;
	delete pCylinderVA_;
	delete pCylinderTexture_;

	delete pConeVB_;
	delete pConeVA_;
	delete pConeTexture_;
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
	
	const math::matrix4f &matSphereWorld = math::make_scale(4.0f) * math::make_translation(10.0f,0.0f,0.0f);
	shader_.program().begin();
	shader_.program().sendUniform("in_UsePointLight", 0);
	shader_.program().sendUniformMatrix( "in_World", matSphereWorld);
	shader_.program().sendUniformMatrix( "in_WVP", matSphereWorld * matVP);
	pSphereTexture_->bind();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	pSphereVA_->draw();
	//glDisable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	static const math::matrix4f &matCubeWorld = math::make_scale(2.0f) * math::make_translation(-10.0f,0.0f,0.0f);

	shader_.program().sendUniformMatrix( "in_WVP", matCubeWorld * matVP);
	pCubeTexture_->bind();
	pCubeVA_->draw();

	shader_.program().end();

	shaderColored_.program().begin();

	const float scy = 0.05f;

	static const math::matrix4f S = math::make_scale(5.f);

	static const math::matrix4f &matCylinderWorldX = math::make_scale(scy,0.75f,scy) * math::make_rotation(math::vector3f::y_axis, math::vector3f::x_axis) * math::make_translation(0.5f*0.75f,0.0f,0.0f);

	static const math::matrix4f &matCylinderWorldY = math::make_scale(scy,0.75f,scy) * math::make_translation(0.0f,0.5f*0.75f,0.0f);

	static const math::matrix4f &matCylinderWorldZ = math::make_scale(scy,0.75f,scy) * math::make_rotation(math::vector3f::y_axis, math::vector3f::z_axis) * math::make_translation(0.0f,0.0f,0.5f*0.75f);

	pCylinderTexture_->bind();
	shaderColored_.program().sendUniformMatrix( "in_World", matCylinderWorldX*S);
	shaderColored_.program().sendUniformMatrix( "in_WVP", matCylinderWorldX * S*matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::x_axis);
	pCylinderVA_->draw();
	shaderColored_.program().sendUniformMatrix( "in_World", matCylinderWorldY*S);
	shaderColored_.program().sendUniformMatrix( "in_WVP", matCylinderWorldY * S*matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::y_axis);
	pCylinderVA_->draw();
	shaderColored_.program().sendUniformMatrix( "in_World", matCylinderWorldZ*S);
	shaderColored_.program().sendUniformMatrix( "in_WVP", matCylinderWorldZ * S*matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::z_axis);
	pCylinderVA_->draw();


	const float sco = 0.15f;

	static const math::matrix4f &matConeWorldX = math::make_scale(sco,0.25f,sco) * math::make_rotation(math::vector3f::y_axis, math::vector3f::x_axis) * math::make_translation(0.75f,0.0f,0.0f);

	static const math::matrix4f &matConeWorldY = math::make_scale(sco,0.25f,sco) * math::make_translation(0.0f,0.75f,0.0f);

	static const math::matrix4f &matConeWorldZ = math::make_scale(sco,0.25f,sco) * math::make_rotation(math::vector3f::y_axis, math::vector3f::z_axis) * math::make_translation(0.0f,0.0f,0.75f);

	pConeTexture_->bind();
	shaderColored_.program().sendUniform("in_Color", math::vector3f::x_axis);
	shaderColored_.program().sendUniformMatrix( "in_World", matConeWorldX*S);
	shaderColored_.program().sendUniformMatrix( "in_WVP", matConeWorldX * S*matVP);
	pConeVA_->draw();
	shaderColored_.program().sendUniform("in_Color", math::vector3f::y_axis);
	shaderColored_.program().sendUniformMatrix( "in_World", matConeWorldY*S);
	shaderColored_.program().sendUniformMatrix( "in_WVP", matConeWorldY * S*matVP);
	pConeVA_->draw();
	shaderColored_.program().sendUniform("in_Color", math::vector3f::z_axis);
	shaderColored_.program().sendUniformMatrix( "in_World", matConeWorldZ*S);
	shaderColored_.program().sendUniformMatrix( "in_WVP", matConeWorldZ * S*matVP);
	pConeVA_->draw();

	static const math::matrix4f &matCenter = math::make_scale(scy*2.0f);
	shaderColored_.program().sendUniform("in_Color", math::vector3f(1,1,1));
	shaderColored_.program().sendUniformMatrix( "in_World", matCenter*S);
	shaderColored_.program().sendUniformMatrix( "in_WVP", matCenter*S*matVP);
	pCubeVA_->draw();

	shaderColored_.program().end();

	opengl::texture::unbind();

	glDisable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------------------------------------------