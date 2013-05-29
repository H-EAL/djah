#include "deferred_shading_test.hpp"
#include "djah/3d/primitives.hpp"

using namespace djah;

static const float s = 4.0f;
//--------------------------------------------------------------------------------------------------
DeferredShadingTest::DeferredShadingTest(djah::system::device_sptr pDevice, const djah::system::input::gamepad &g, Camera &cam)
	: test_base(pDevice)
	, cam_(cam)
	, cow_("cow")
	, gamepad_(g)
	, pVertexBuffer_(nullptr)
	, pFloorTexture_(nullptr)
	, deferredFBO_()
	, depthFB_(GL_DEPTH_COMPONENT32F, pDevice->config().width, pDevice->config().height)
	, currentBuffer_(eGB_Positions)
	, shaderProgram_("textured")
	, deferredProgram_("deferred_geometry")
	, compositorProgram_("deferred_compositor")
{
	const float w = static_cast<float>(pDevice_->config().width);
	const float h = static_cast<float>(pDevice_->config().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);
	matOrthoProj_		= math::make_orthographic_projection(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	const std::vector<d3d::primitives::triangle> &triangles = d3d::primitives::cube().construct();
	pVertexBuffer_ = new opengl::vertex_buffer(triangles.size() * sizeof(d3d::primitives::triangle), opengl::eBU_StaticDraw);
	pVertexBuffer_->write(&triangles[0], triangles.size());

	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>();

	vertexArray_.addVertexBuffer(pVertexBuffer_, vertexFormat);
	vertexArray_.setVertexCount(triangles.size()*3);
	vertexArray_.init(deferredProgram_.program());

	pFloorTexture_ = d3d::texture_manager::get().find("wood.jpg");

	for(int i = 0; i < eGB_Count; ++i)
	{
		pGeometryBuffers_[i] = new opengl::texture(GL_RGB32F, pDevice_->config().width, pDevice_->config().height);
		pGeometryBuffers_[i]->bind();
		pGeometryBuffers_[i]->setNoFiltering();
		pGeometryBuffers_[i]->setPixelBuffer(GL_RGB, GL_FLOAT, nullptr);
	}

	depthFB_.bind();
	depthFB_.setNoFiltering();
	depthFB_.setPixelBuffer(GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	deferredFBO_.bindWriting();
	GLenum drawBuffers[eGB_Count];
	for(int i = 0; i < eGB_Count; ++i)
	{
		drawBuffers[i] = (GLenum)deferredFBO_.attach(*pGeometryBuffers_[i]);
	}
	deferredFBO_.attach(depthFB_);

	glDrawBuffers(eGB_Count, drawBuffers);

	deferredFBO_.unbindWriting();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
DeferredShadingTest::~DeferredShadingTest()
{
	delete pVertexBuffer_;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void DeferredShadingTest::onInit()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void DeferredShadingTest::onExit()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void DeferredShadingTest::update(float dt)
{
	if( gamepad_.getButton(system::input::eX360_A).pressed() )
	{
		currentBuffer_ = (currentBuffer_ + 1) % eGB_Count;
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void DeferredShadingTest::draw()
{
	const math::matrix4f &matViewProj  = math::make_look_at(cam_.eye(), cam_.center(), cam_.up()) * matPerspectiveProj_;

	renderGeometryPass(matViewProj);
	renderFinalPass(matOrthoProj_);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void DeferredShadingTest::renderGeometryPass(const math::matrix4f &matViewProj)
{
	deferredFBO_.bindWriting();
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	deferredProgram_.program().begin();

	static const math::matrix4f matWorldFlr = math::make_scale(4.0f, 0.05f, 4.0f);
	deferredProgram_.program().sendUniform( "in_World", matWorldFlr );
	deferredProgram_.program().sendUniform( "in_WVP", matWorldFlr * matViewProj );
	deferredProgram_.program().sendUniform( "in_DiffuseSampler", 0 );
	pFloorTexture_->bind();
	vertexArray_.draw();

	static const math::matrix4f matWorldCow = math::matrix4f::identity;
	deferredProgram_.program().sendUniform( "in_World", matWorldCow );
	deferredProgram_.program().sendUniform( "in_WVP", matWorldCow * matViewProj );
	cow_.tex_->bind();
	cow_.va_.draw();

	deferredProgram_.program().end();

	opengl::texture::unbind();

	deferredFBO_.unbindWriting();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void DeferredShadingTest::renderLightPass(const math::matrix4f &matViewProj)
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	opengl::frame_buffer::bind_default_frame_buffer();
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_BLEND);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void DeferredShadingTest::renderFinalPass(const math::matrix4f &matViewProj)
{
	opengl::frame_buffer::bind_default_frame_buffer();

	const math::vector2f screenSize( static_cast<float>(pDevice_->config().width), static_cast<float>(pDevice_->config().height) );

	static const math::matrix4f &sc = math::make_scale(2.0f);

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	compositorProgram_.program().begin();
	compositorProgram_.program().sendUniform( "in_WVP", sc*matViewProj );
	//compositorProgram_.program().sendUniform( "in_PositionSampler", eGB_Positions );
	compositorProgram_.program().sendUniform( "in_DiffuseSampler",  eGB_Diffuse   );
	compositorProgram_.program().sendUniform( "in_NormalSampler",   eGB_Normals   );
	//compositorProgram_.program().sendUniform( "in_TexCoordSampler", eGB_TexCoords );
	compositorProgram_.program().sendUniform( "in_ScreenSize", screenSize );
	
	for(int i = 0; i < eGB_Count; ++i)
	{
		opengl::texture::set_active_unit(i);
		pGeometryBuffers_[i]->bind();
	}

	vertexArray_.draw();
	compositorProgram_.program().end();

	for(int i = eGB_Count-1; i >= 0; --i)
	{
		opengl::texture::set_active_unit(i);
		pGeometryBuffers_[i]->unbind();
	}
}
//--------------------------------------------------------------------------------------------------


/*
//--------------------------------------------------------------------------------------------------
void DeferredShadingTest::renderTest(const math::matrix4f &matViewProj)
{
	opengl::frame_buffer::bind_default_frame_buffer();
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//pVertexArray_->init(shaderProgram_);
	shaderProgram_.begin();
	shaderProgram_.sendUniform( "in_DiffuseSampler", 0 );

	shaderProgram_.sendUniformMatrix( "in_WVP", matViewProj );
	pFloorTexture_->bind();
	pVertexArray_->draw();
	pFloorTexture_->unbind();

	shaderProgram_.sendUniformMatrix( "in_WVP", matWorldCow * matViewProj );
	cow_.tex_->bind();
	cow_.va_->draw();
	cow_.tex_->unbind();


	static const math::matrix4f matWorldPos = math::make_scale(1.78f*0.25f, 0.25f, 0.25f) * math::make_rotation(math::deg_to_rad(-90.0f), math::vector3f::x_axis) * math::make_translation(-6.0f,s/2.0f,-s);
	shaderProgram_.sendUniformMatrix( "in_WVP", matWorldPos * matViewProj );
	pGeometryBuffers_[eGB_Positions]->bind();
	pVertexArray_->draw();

	static const math::matrix4f matWorldDif = math::make_scale(1.78f*0.25f, 0.25f, 0.25f) * math::make_rotation(math::deg_to_rad(-90.0f), math::vector3f::x_axis) * math::make_translation(-2.0f,s/2.0f,-s);
	shaderProgram_.sendUniformMatrix( "in_WVP", matWorldDif * matViewProj );
	pGeometryBuffers_[eGB_Diffuse]->bind();
	pVertexArray_->draw();

	static const math::matrix4f matWorldNorm = math::make_scale(1.78f*0.25f, 0.25f, 0.25f) * math::make_rotation(math::deg_to_rad(-90.0f), math::vector3f::x_axis) * math::make_translation(2.0f,s/2.0f,-s);
	shaderProgram_.sendUniformMatrix( "in_WVP", matWorldNorm * matViewProj );
	pGeometryBuffers_[eGB_Normals]->bind();
	pVertexArray_->draw();

	static const math::matrix4f matWorldTc = math::make_scale(1.78f*0.25f, 0.25f, 0.25f) * math::make_rotation(math::deg_to_rad(-90.0f), math::vector3f::x_axis) * math::make_translation(6.0f,s/2.0f,-s);
	shaderProgram_.sendUniformMatrix( "in_WVP", matWorldTc * matViewProj );
	pGeometryBuffers_[eGB_TexCoords]->bind();
	pVertexArray_->draw();

	static const math::matrix4f matWorldDep = math::make_scale(1.78f*0.25f, 0.25f, 0.25f) * math::make_rotation(math::deg_to_rad(-90.0f), math::vector3f::x_axis) * math::make_translation(0.0f,s,-s);
	shaderProgram_.sendUniformMatrix( "in_WVP", matWorldDep * matViewProj );
	depthFB_.bind();
	pVertexArray_->draw();
	depthFB_.unbind();

	shaderProgram_.end();
}
//--------------------------------------------------------------------------------------------------
*/