#include "basic_test.hpp"
#include "djah/types.hpp"
#include "resource_finder.hpp"
#include "djah/3d/primitives.hpp"
#include "djah/resources/media_manager.hpp"
#include "djah/core/randomizer.hpp"
#include "djah/core/noise.hpp"

using namespace djah;
using namespace d3d;

//--------------------------------------------------------------------------------------------------
BasicTest::BasicTest(djah::system::device_ptr pDevice, Camera &cam)
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
	, pVB_(nullptr)
	, pTB_(nullptr)
	, pNoiseTexture_(nullptr)
	, shader_("lit_textured")
	, shaderColored_("uniform_color")
	, shaderBatch_("batch")
	, nbTrans_(3)
{
	const float w = static_cast<float>(pDevice_->videoConfig().width);
	const float h = static_cast<float>(pDevice_->videoConfig().height);
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

	/*
	const std::vector<primitives::triangle> &triangles = primitives::cube().construct(10, 10);
	pVB_ = new opengl::vertex_buffer(triangles.size() * sizeof(primitives::triangle), opengl::eBU_StaticDraw);
	pVB_->write(&triangles[0], triangles.size());

	const float translations[]=
	{
		0.0f, 1.0f,0.0f,0.0f,
		1.0f, 0.0f,1.0f,0.0f,
		2.0f, 0.0f,0.0f,1.0f
	};
	pTB_ = new opengl::vertex_buffer(sizeof(translations), opengl::eBU_StaticDraw);
	pTB_->write(translations);
	*/
	
	batcher_.init<primitives::sphere>(15,15);

	math::transformation_f t;
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

	const int ww = 512;
	const int hh = 512;
	const int octaves = 7;
	const float p = 0.50f;
	const float zoom = 75.0f;

	std::vector<primitives::triangle> tris(ww*hh*2);

	byte n[ww][hh];
	int tt = 0;
	for(int x = 0; x < ww; ++x)
	{
		for(int z = 0; z < hh; ++z)
		{
			float totalNoise = 0.0f;
			for(int a = 0; a < octaves; ++a)
			{
				float freq = pow(2.0f,a);
				float ampl = pow(p,a);
				totalNoise += noise<float>::noise2((float)x*freq/zoom, ((float)z/zoom*freq))*ampl;
			}
			n[x][z] = static_cast<byte>(math::clamp(totalNoise * 128.0f + 128.0f, 0.0f, 255.0f));
		}
	}

	for(int x = 0; x < ww-1; ++x)
	{
		for(int z = 0; z < hh-1; ++z)
		{
			static const float maxHeight = 50.0f;

			const float y = ((float)n[x][z] - 128.0f) / 255.0f       * maxHeight;
			const float yx = ((float)n[x+1][z] - 128.0f) / 255.0f    * maxHeight;
			const float yz = ((float)n[x][z+1] - 128.0f) / 255.0f    * maxHeight;
			const float yxz = ((float)n[x+1][z+1] - 128.0f) / 255.0f * maxHeight;

			tris[tt].setPoints(math::point3f(x,y,z), math::point3f(x,yz,z+1), math::point3f(x+1,yx,z));
			tris[tt].setNormal(math::vector3f(0,1,0));
			tris[tt].setTextureCoordinates
			(
				math::point2f(x/(float)ww,     1.0f-z/(float)hh),
				math::point2f(x/(float)ww,     1.0f-(z+1)/(float)hh),
				math::point2f((x+1)/(float)ww, 1.0f-z/(float)hh)
			);

			++tt;

			tris[tt].setPoints(math::point3f(x+1,yx,z), math::point3f(x,yz,z+1), math::point3f(x+1,yxz,z+1));
			tris[tt].setNormal(math::vector3f(0,1,0));
			tris[tt].setTextureCoordinates
				(
				math::point2f((x+1)/(float)ww,     1.0f-z/(float)hh),
				math::point2f(x/(float)ww,     1.0f-(z+1)/(float)hh),
				math::point2f((x+1)/(float)ww, 1.0f-(z+1)/(float)hh)
				);

			++tt;
		}
	}


	pTerrainVB_ = new opengl::vertex_buffer(tris.size() * sizeof(primitives::triangle), opengl::eBU_StaticDraw);
	pTerrainVB_->write(&tris[0], tris.size());
	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>();

	terrainVA_.addVertexBuffer(pTerrainVB_, vertexFormat);
	terrainVA_.setVertexCount(tris.size()*3);
	terrainVA_.init(shader_.program());

	pNoiseTexture_ = new opengl::texture(GL_DEPTH_COMPONENT, ww, hh);
	pNoiseTexture_->bind();
	pNoiseTexture_->setBestFiltering();
	pNoiseTexture_->setPixelBuffer(GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, &n[0][0]);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
template<typename P>
void BasicTest::initPrimitive(
	djah::opengl::vertex_buffer *&pVB,
	djah::opengl::vertex_array &pVA,
	const std::string &textureFile,
	djah::d3d::texture_ptr &pTex,
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
	delete pNoiseTexture_;
	delete pTerrainVB_;
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
	//batcher_.draw(matVP);


	shader_.program().begin();
	shader_.program().sendUniform("in_UsePointLight", 0);
	static const math::matrix4f &matCubeWorld = math::make_scale(10.0f) * math::make_translation(0.0f,0.0f,0.0f);
	static const math::matrix4f &matTerrWorld = math::make_translation(-256.0f,0.0f,-256.0f) * math::make_scale(1.0f);
	shader_.program().sendUniform( "in_WVP", matCubeWorld * matVP);
	pNoiseTexture_->bind();
	cubeVA_.draw();
	shader_.program().sendUniform( "in_WVP", matTerrWorld * matVP);
	pCubeTexture_->bind();
	terrainVA_.draw();
	shader_.program().end();

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

	shaderColored_.program().begin();
	shaderColored_.program().sendUniform( "in_Lit", true);
	drawTranslationGizmo(matVP);
	drawRotationGizmo(matVP);


	shaderColored_.program().end();

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