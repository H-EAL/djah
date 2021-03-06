#include "basic_test.hpp"
#include "djah/types.hpp"
#include "resource_finder.hpp"
#include "djah/3d/primitives.hpp"
#include "djah/resources/asset_finder.hpp"
#include "djah/core/randomizer.hpp"
#include "djah/core/noise.hpp"

using namespace djah;
using namespace d3d;

//--------------------------------------------------------------------------------------------------
BasicTest::BasicTest(djah::system::device_sptr pDevice, Camera &cam)
	: test_base(pDevice)
	, cam_(cam)
	, pCubeVB_(nullptr)
	, pCubeTexture_(nullptr)
	, pSphereVB_(nullptr)
	, pSphereTexture_(nullptr)
	, pCylinderVB_(nullptr)
	, pCylinderTexture_(nullptr)
	, pConeVB_(nullptr)
	, pConeTexture_(nullptr)
	, pRibbonVB_(nullptr)
	, pRibbonTexture_(nullptr)
	, shader_("lit_textured")
	, shaderColored_("uniform_color")
	, shaderBatch_("batch")
{
	const float w = static_cast<float>(pDevice_->config().width);
	const float h = static_cast<float>(pDevice_->config().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);

	initPrimitive<primitives::cube>
	(
		pCubeVB_,
		cubeVA_,
		"terrain2.png",
		pCubeTexture_,
		10
	);
	
	initPrimitive<primitives::sphere>
	(
		pSphereVB_,
		sphereVA_,
		"earth.jpg",
		pSphereTexture_,
		20,20
	);

	initPrimitive<primitives::cylinder>
	(
		pCylinderVB_,
		cylinderVA_,
		"earth.jpg",
		pCylinderTexture_,
		20
	);

	initPrimitive<primitives::cone>
	(
		pConeVB_,
		coneVA_,
		"earth.jpg",
		pConeTexture_,
		20
	);

	initPrimitive<primitives::ribbon>
	(
		pRibbonVB_,
		ribbonVA_,
		"earth.jpg",
		pRibbonTexture_,
		50
	);
	
	batcher_.init<primitives::sphere>(15,15);

	math::transform_f t;
	t.uniformScale(0.5f);
	int m = -10;
	int M = 10;
	for( int i = m; i < M; ++i )
	{
		for( int j = m; j < M; ++j )
		{
			t.setTranslation( math::vector3f((float)i, utils::randomizer::random(1.0f), (float)j) );
			batcher_.add( t, math::vector3f(utils::randomizer::random(1.0f), utils::randomizer::random(1.0f), utils::randomizer::random(1.0f)) );
		}
	}
	/**/
	for( int i = m; i < M; ++i )
	{
		for( int j = m; j < M; ++j )
		{
			if( utils::randomizer::random(100) < 20 )
			{
				batcher_.remove( (j-m)+(i-m)*(M-m-1) );
			}
		}
	}
	/**/

	pCowTexture_ = d3d::texture_manager::get().find("drybed_diffuse01.jpg");
	pGridMesh = find_resource<resources::mesh>("meshes/grid_space_dirt_01.mesh");
	pBlockMesh = find_resource<resources::mesh>("meshes/block_dirt_01.mesh");
	pGridMesh->init(shader_.program());
	pBlockMesh->init(shader_.program());
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
template<typename P>
void BasicTest::initPrimitive(
	djah::opengl::vertex_buffer *&pVB,
	djah::opengl::vertex_array &pVA,
	const std::string &textureFile,
	djah::d3d::texture_sptr &pTex,
	int level1, int level2)
{
	const std::vector<primitives::triangle> &triangles = P().construct(level1, level2);

	pVB = new opengl::vertex_buffer(triangles.size() * sizeof(primitives::triangle), opengl::eBU_StaticDraw);
	pVB->write(&triangles[0], triangles.size());

	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>();

	pVA.addVertexBuffer(pVB, vertexFormat);
	pVA.setVertexCount(triangles.size()*3);
	pVA.init(shader_.program());

	pTex = d3d::texture_manager::get().find(textureFile);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
BasicTest::~BasicTest()
{
	delete pCubeVB_;
	delete pSphereVB_;
	delete pCylinderVB_;
	delete pConeVB_;
	delete pRibbonVB_;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void BasicTest::onInit()
{
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	opengl::frame_buffer::bind_default_frame_buffer();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void BasicTest::onExit()
{
	glDisable(GL_DEPTH_TEST);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const math::matrix4f &matVP = math::make_look_at(cam_.eye(), cam_.center(), cam_.up()) * matPerspectiveProj_;
	const math::matrix4f &in_World = math::matrix4f::identity;
	//batcher_.draw(matVP);

	/**
	const math::matrix4f &matSphereWorld = math::make_scale(4.0f) * math::make_translation(10.0f,0.0f,0.0f);
	shader_.program().begin();
	shader_.program().sendUniform("in_UsePointLight", 0);
	shader_.program().sendUniformMatrix( "in_World", matSphereWorld);
	shader_.program().sendUniformMatrix( "in_WVP", matSphereWorld * matVP);
	pSphereTexture_->bind();
	pSphereVA_->draw();
	
	static const math::matrix4f &matCubeWorld = math::make_scale(2.0f) * math::make_translation(-10.0f,0.0f,0.0f);

	shader_.program().sendUniformMatrix( "in_WVP", matCubeWorld * matVP);
	pCubeTexture_->bind();
	pCubeVA_->draw();

	shader_.program().end();
	**/

	shader_.program().begin();
	shader_.program().sendUniform( "in_UsePointLight", false);
	//drawTranslationGizmo(matVP);
	//drawRotationGizmo(matVP);

	shader_.program().sendUniform( "in_World", in_World);
	shader_.program().sendUniform( "in_WVP", in_World * matVP);
	shader_.program().sendUniform("in_DiffuseSampler", 0);
	//shader_.program().sendUniform("in_Color", math::vector3f::x_axis);
	pCowTexture_->bind();
	pGridMesh->draw();
	pBlockMesh->draw();
	pCowTexture_->unbind();

	shader_.program().end();

	opengl::texture::unbind();
	/**/
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void BasicTest::drawRotationGizmo(const math::matrix4f &matVP)
{
	const float s = 0.05f;
	static const math::matrix4f &matX = math::make_scale(1.0f,s,1.0f) * math::make_rotation(math::vector3f::y_axis, math::vector3f::x_axis);
	static const math::matrix4f &matY = math::make_scale(1.0f+0.002f,s,1.0f+0.002f);
	static const math::matrix4f &matZ = math::make_scale(1.0f+0.005f,s,1.0f+0.005f) * math::make_rotation(math::vector3f::y_axis, math::vector3f::z_axis);

	// X
	shaderColored_.program().sendUniform( "in_World", matX);
	shaderColored_.program().sendUniform( "in_WVP", matX * matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::x_axis);
	ribbonVA_.draw();
	// Y
	shaderColored_.program().sendUniform( "in_World", matY);
	shaderColored_.program().sendUniform( "in_WVP", matY * matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::y_axis);
	ribbonVA_.draw();
	//Z
	shaderColored_.program().sendUniform( "in_World", matZ);
	shaderColored_.program().sendUniform( "in_WVP", matZ * matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::z_axis);
	ribbonVA_.draw();

	const float sc = 0.1f;
	static const math::matrix4f &matCX = math::make_scale(sc) * math::make_rotation(math::vector3f::y_axis, math::vector3f::x_axis) * math::make_translation(0.0f,1.0f,0.0f);
	static const math::matrix4f &matCY = math::make_scale(sc) * math::make_translation(0.0f,0.0f,1.0f);
	static const math::matrix4f &matCZ = math::make_scale(sc) * math::make_rotation(math::vector3f::y_axis, math::vector3f::z_axis) * math::make_translation(1.0f,0.0f,0.0f);

	// X
	shaderColored_.program().sendUniform( "in_World", matCX);
	shaderColored_.program().sendUniform( "in_WVP", matCX * matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::x_axis);
	cubeVA_.draw();
	// Y
	shaderColored_.program().sendUniform( "in_World", matCY);
	shaderColored_.program().sendUniform( "in_WVP", matCY * matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::y_axis);
	cubeVA_.draw();
	//Z
	shaderColored_.program().sendUniform( "in_World", matCZ);
	shaderColored_.program().sendUniform( "in_WVP", matCZ * matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::z_axis);
	cubeVA_.draw();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void BasicTest::drawTranslationGizmo(const math::matrix4f &matVP)
{

	const float scy = 0.05f;

	static const math::matrix4f S = math::make_scale(0.5f);

	static const math::matrix4f &matCylinderWorldX = math::make_scale(scy,0.75f,scy) * math::make_rotation(math::vector3f::y_axis, math::vector3f::x_axis) * math::make_translation(0.5f*0.75f,0.0f,0.0f);

	static const math::matrix4f &matCylinderWorldY = math::make_scale(scy,0.75f,scy) * math::make_translation(0.0f,0.5f*0.75f,0.0f);

	static const math::matrix4f &matCylinderWorldZ = math::make_scale(scy,0.75f,scy) * math::make_rotation(math::vector3f::y_axis, math::vector3f::z_axis) * math::make_translation(0.0f,0.0f,0.5f*0.75f);

	shaderColored_.program().sendUniform( "in_World", matCylinderWorldX*S);
	shaderColored_.program().sendUniform( "in_WVP", matCylinderWorldX * S*matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::x_axis);
	cylinderVA_.draw();
	shaderColored_.program().sendUniform( "in_World", matCylinderWorldY*S);
	shaderColored_.program().sendUniform( "in_WVP", matCylinderWorldY * S*matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::y_axis);
	cylinderVA_.draw();
	shaderColored_.program().sendUniform( "in_World", matCylinderWorldZ*S);
	shaderColored_.program().sendUniform( "in_WVP", matCylinderWorldZ * S*matVP);
	shaderColored_.program().sendUniform("in_Color", math::vector3f::z_axis);
	cylinderVA_.draw();


	const float sco = 0.15f;

	static const math::matrix4f &matConeWorldX = math::make_scale(sco,0.25f,sco) * math::make_rotation(math::vector3f::y_axis, math::vector3f::x_axis) * math::make_translation(0.75f,0.0f,0.0f);

	static const math::matrix4f &matConeWorldY = math::make_scale(sco,0.25f,sco) * math::make_translation(0.0f,0.75f,0.0f);

	static const math::matrix4f &matConeWorldZ = math::make_scale(sco,0.25f,sco) * math::make_rotation(math::vector3f::y_axis, math::vector3f::z_axis) * math::make_translation(0.0f,0.0f,0.75f);

	shaderColored_.program().sendUniform("in_Color", math::vector3f::x_axis);
	shaderColored_.program().sendUniform( "in_World", matConeWorldX*S);
	shaderColored_.program().sendUniform( "in_WVP", matConeWorldX * S*matVP);
	coneVA_.draw();
	shaderColored_.program().sendUniform("in_Color", math::vector3f::y_axis);
	shaderColored_.program().sendUniform( "in_World", matConeWorldY*S);
	shaderColored_.program().sendUniform( "in_WVP", matConeWorldY * S*matVP);
	coneVA_.draw();
	shaderColored_.program().sendUniform("in_Color", math::vector3f::z_axis);
	shaderColored_.program().sendUniform( "in_World", matConeWorldZ*S);
	shaderColored_.program().sendUniform( "in_WVP", matConeWorldZ * S*matVP);
	coneVA_.draw();

	static const math::matrix4f &matCenter = math::make_scale(scy*4.0f);
	shaderColored_.program().sendUniform("in_Color", math::vector3f(1,1,1));
	shaderColored_.program().sendUniform( "in_World", matCenter*S);
	shaderColored_.program().sendUniform( "in_WVP", matCenter*S*matVP);
	cubeVA_.draw();

}
//--------------------------------------------------------------------------------------------------