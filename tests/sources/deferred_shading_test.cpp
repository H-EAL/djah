#include "deferred_shading_test.hpp"

using namespace djah;

static const float s = 4.0f;
//--------------------------------------------------------------------------------------------------
DeferredShadingTest::DeferredShadingTest(djah::system::device_ptr pDevice, const djah::system::input::gamepad &g, Camera &cam)
	: test_base(pDevice)
	, cam_(cam)
	, cow_("cow")
	, gamepad_(g)
	, indexBuffer_(2*3, opengl::eBU_StaticDraw)
	, vertexBuffer_( (3+3+2)*4*sizeof(float), opengl::eBU_StaticDraw)
	, pVertexArray_(nullptr)
	, pFloorTexture_(nullptr)
	, deferredFBO_()
	, depthFB_(GL_DEPTH_COMPONENT32F, pDevice->videoConfig().width, pDevice->videoConfig().height)
	, currentBuffer_(eGB_Positions)
	, shaderProgram_("textured")
	, deferredProgram_("deferred_geometry")
	, compositorProgram_("deferred_compositor")
{
	const float w = static_cast<float>(pDevice_->videoConfig().width);
	const float h = static_cast<float>(pDevice_->videoConfig().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);
	matOrthoProj_		= math::make_orthographic_projection(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	static const float t = 1.0f;
	const float cubeVertices[] = 
	{
		//Position - Normal - TexCoord//
		-1,-1,0,     0,0,1,   0,0,    // 0
		 1,-1,0,     0,0,1,   t,0,    // 1
		 1, 1,0,     0,0,1,   t,t,    // 2
		-1, 1,0,     0,0,1,   0,t,    // 3
	};

	const u8 cubeIndices[] =
	{
		0,3,2, 0,2,1,
	};

	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>();

	indexBuffer_.write(cubeIndices);
	vertexBuffer_.write(cubeVertices);
	pVertexArray_ = new opengl::vertex_array(vertexFormat, &vertexBuffer_, &indexBuffer_);

	{
		opengl::vertex_shader vertexShader( loadShaderSource("shaders/textured.vert") );
		opengl::pixel_shader  pixelShader(  loadShaderSource("shaders/textured.frag") );

		if( vertexShader.compile() && pixelShader.compile() )
		{
			shaderProgram_.attach(vertexShader);
			shaderProgram_.attach(pixelShader);
			if( shaderProgram_.link() )
			{
				pVertexArray_->init(shaderProgram_);
			}
		}
	}

	{
		opengl::vertex_shader vertexShader( loadShaderSource("shaders/deferred_geometry.vert") );
		opengl::pixel_shader  pixelShader(  loadShaderSource("shaders/deferred_geometry.frag") );

		if( vertexShader.compile() && pixelShader.compile() )
		{
			deferredProgram_.attach(vertexShader);
			deferredProgram_.attach(pixelShader);
			if( deferredProgram_.link() )
			{
				pVertexArray_->init(deferredProgram_);
			}
		}
	}

	{
		opengl::vertex_shader vertexShader( loadShaderSource("shaders/deferred_compositor.vert") );
		opengl::pixel_shader  pixelShader(  loadShaderSource("shaders/deferred_compositor.frag") );

		if( vertexShader.compile() && pixelShader.compile() )
		{
			compositorProgram_.attach(vertexShader);
			compositorProgram_.attach(pixelShader);
			if( compositorProgram_.link() )
			{
				pVertexArray_->init(compositorProgram_);
			}
		}
	}

	resources::image_ptr floorImg = find_resource<resources::image>("textures/wood.jpg");
	if( floorImg )
	{
		pFloorTexture_ = new opengl::texture(GL_RGB, floorImg->width(), floorImg->height());
		pFloorTexture_->bind();
		pFloorTexture_->setBestFiltering();
		pFloorTexture_->setPixelBuffer(GL_BGR, GL_UNSIGNED_BYTE, floorImg->pixels());
	}

	for(int i = 0; i < eGB_Count; ++i)
	{
		pGeometryBuffers_[i] = new opengl::texture(GL_RGB32F, pDevice_->videoConfig().width, pDevice_->videoConfig().height);
		pGeometryBuffers_[i]->bind();
		pGeometryBuffers_[i]->setNoFiltering();
		pGeometryBuffers_[i]->setPixelBuffer(GL_RGB, GL_FLOAT, nullptr);
	}

	depthFB_.bind();
	depthFB_.setNoFiltering();
	depthFB_.setPixelBuffer(GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	deferredFBO_.bind();
	GLenum drawBuffers[eGB_Count];
	for(int i = 0; i < eGB_Count; ++i)
	{
		drawBuffers[i] = (GLenum)deferredFBO_.attach(*pGeometryBuffers_[i]);
	}
	deferredFBO_.attach(depthFB_);

	glDrawBuffers(eGB_Count, drawBuffers);

	deferredFBO_.unbind();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
DeferredShadingTest::~DeferredShadingTest()
{
	delete pVertexArray_;
	delete pFloorTexture_;
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
	glDisable(GL_CULL_FACE);

	const math::matrix4f &matViewProj  = math::make_look_at(cam_.eye(), cam_.center(), cam_.up()) * matPerspectiveProj_;

	renderGeometryPass(matViewProj);
	renderFinalPass(matOrthoProj_);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void DeferredShadingTest::renderGeometryPass(const math::matrix4f &matViewProj)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	deferredFBO_.bind();
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//pVertexArray_->init(deferredProgram_);
	//cow_.va_->init(deferredProgram_);

	deferredProgram_.begin();

	static const math::matrix4f matWorldFlr = math::make_scale(4.0f, 4.0f, 4.0f) * math::make_rotation(math::deg_to_rad(-90.0f), math::vector3f::x_axis);
	deferredProgram_.sendUniformMatrix( "in_World", matWorldFlr );
	deferredProgram_.sendUniformMatrix( "in_WVP", matWorldFlr * matViewProj );
	pFloorTexture_->bind();
	deferredProgram_.sendUniform( "in_DiffuseSampler", 0 );
	pVertexArray_->draw();
	pFloorTexture_->unbind();

	static const math::matrix4f matWorldCow = math::matrix4f::identity;//math::make_scale(0.05f, 0.05f, 0.05f) * math::make_translation(0.0f,2.0f,0.0f);// * math::make_rotation(math::deg_to_rad(-90.0f), math::vector3f::x_axis);
	deferredProgram_.sendUniformMatrix( "in_World", matWorldCow );
	deferredProgram_.sendUniformMatrix( "in_WVP", matWorldCow * matViewProj );
	cow_.tex_->bind();
	cow_.va_->draw();
	cow_.tex_->unbind();

	deferredProgram_.end();

	deferredFBO_.unbind();

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
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
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void DeferredShadingTest::renderFinalPass(const math::matrix4f &matViewProj)
{
	opengl::frame_buffer::bind_default_frame_buffer();

	const math::vector2f screenSize( static_cast<float>(pDevice_->videoConfig().width), static_cast<float>(pDevice_->videoConfig().height) );

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	compositorProgram_.begin();
	compositorProgram_.sendUniform( "in_PositionSampler", 0 );
	compositorProgram_.sendUniform( "in_DiffuseSampler",  1 );
	compositorProgram_.sendUniform( "in_NormalSampler",   2 );
	compositorProgram_.sendUniform( "in_TexCoordSampler", 3 );
	compositorProgram_.sendUniform( "in_ScreenSize", screenSize );
	compositorProgram_.sendUniformMatrix( "in_WVP", matViewProj );
	
	for(int i = 0; i < eGB_Count; ++i)
	{
		opengl::texture::set_active_unit(i);
		pGeometryBuffers_[i]->bind();
	}

	pVertexArray_->draw();

	for(int i = eGB_Count-1; i >= 0; --i)
	{
		opengl::texture::set_active_unit(i);
		pGeometryBuffers_[i]->unbind();
	}

	compositorProgram_.end();
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