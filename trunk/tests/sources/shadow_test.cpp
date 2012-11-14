#include "shadow_test.hpp"

#include <djah/opengl.hpp>
#include <djah/3d/text.hpp>
#include <djah/3d/font_engine.hpp>

#include <djah/system/video_config.hpp>
#include <djah/system/device.hpp>
#include <djah/system/driver_base.hpp>

#include <djah/debug/log.hpp>

#include <djah/core/time/timer.hpp>

#include <djah/core/murmur_hash.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>
#include <djah/filesystem/memory_stream.hpp>
#include <djah/filesystem/url.hpp>

#include <djah/resources/media_manager.hpp>
#include <djah/resources/resource_manager.hpp>
#include <djah/resources/image_loader.hpp>

#include <djah/dataobject/registry.hpp>

#include "test.hpp"
#include "model.hpp"
#include "profiler.hpp"

using namespace djah;

#define SH_VP_W 1 << 10
#define SH_VP_H 1 << 10

//--------------------------------------------------------------------------------------------------
ShadowTest::ShadowTest(djah::system::device_ptr pDevice,
	djah::system::input::mouse    &m, 
	djah::system::input::keyboard &k,
	djah::system::input::gamepad  &g,
	Camera &c)
	: test_base(pDevice)
	, spotPosition_()
	, mouse_(m)
	, keyboard_(k)
	, gamepad_(g)
	, cam(c)
	, pShadowMap_(nullptr)
	, pFloorTexture_(nullptr)
	, pVertexArray_(nullptr)
	, pVertexBuffer_(nullptr)
{
	init();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
ShadowTest::~ShadowTest()
{
	delete pFBO_;
	delete pShadowMap_;
	delete pFloorTexture_;
	delete pVertexArray_;
	delete pVertexBuffer_;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void ShadowTest::init()
{
	const float w = static_cast<float>(pDevice_->videoConfig().width);
	const float h = static_cast<float>(pDevice_->videoConfig().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);

	astroboy_	= new model("astroboy");
	cow_		= new model("cow");
	cthulhu_	= new model("cthulhu");

	matShadowPerspectiveProj_ = math::make_perspective_projection(60.0f, 1.0f, 0.5f, 50.f);

	pShadowMap_ = new djah::opengl::texture(GL_DEPTH_COMPONENT, SH_VP_W, SH_VP_H);
	pShadowMap_->bind();
	pShadowMap_->setNoFiltering();
	pShadowMap_->setPixelBuffer(GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	/*
	pShadowMap_->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	pShadowMap_->unbind();
	*/

	pFBO_ = new djah::opengl::frame_buffer;
	pFBO_->bind();
	pFBO_->attach(*pShadowMap_);
	pFBO_->unbind();

	const float w2 = 10.0f;
	const float h2  = 0.0f;
	const float d2 = 10.0f;
	const float rs = 20.0f;
	const float rt = 20.0f;
	float floorVB[] =
	{
		//p0
		-w2, h2,  d2,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f,

		//p2
		w2, h2, -d2,
		0.0f, 1.0f, 0.0f,
		rs, rt,

		//p1
		-w2, h2, -d2,
		0.0f, 1.0f, 0.0f,
		0.0f, rt,

		//p0
		-w2, h2,  d2,
		0.0f, 1.0f, 0.0f,	
		0.0f, 0.0f,

		//p3
		w2, h2,  d2,
		0.0f, 1.0f, 0.0f,
		rs, 0.0f,

		//p2
		w2, h2, -d2,
		0.0f, 1.0f, 0.0f,
		rs, rt,
	};

	pVertexBuffer_ = new djah::opengl::vertex_buffer(sizeof(floorVB), opengl::eBU_StaticDraw);
	pVertexBuffer_->write(floorVB);

	opengl::vertex_format vertexFormat;
	vertexFormat.record()
		<< opengl::format::position<3,float>()
		<< opengl::format::normal<3,float>()
		<< opengl::format::tex_coord<2,float>();

	pVertexArray_ = new djah::opengl::vertex_array(vertexFormat, pVertexBuffer_);

	resources::image_ptr floorImg = find_resource<resources::image>("textures/wood.jpg");
	if( floorImg )
	{
		pFloorTexture_ = new djah::opengl::texture(GL_RGB, floorImg->width(), floorImg->height());
		pFloorTexture_->bind();
		pFloorTexture_->setBestFiltering();
		pFloorTexture_->setPixelBuffer(GL_BGR, GL_UNSIGNED_BYTE, floorImg->pixels());
	}

	pVertexArray_->init( *cow_->sp_ );
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void ShadowTest::update(float dt)
{
	/*
	static bool wasDown = false;
	if( !wasDown && keyboard_.isKeyDown(djah::system::input::eKC_SPACE) )
	{
		astroboy_->compileShader();
		cow_->compileShader();
		cthulhu_->compileShader();
		pVertexArray_->init( *cow_->sp_ );

		time::timer t;
		dataobject::default_registry::get().reload(screenCfgDo_);
		float dt = t.getElapsedTimeSec();
		DJAH_BEGIN_LOG(EWL_MEDIUM) << "Reloading took " << dt << " s" << DJAH_END_LOG();
		device_->setVSync( screenCfgDo_->get<bool>("vsync") );
	}
	wasDown = keyboard_.isKeyDown(djah::system::input::eKC_SPACE);
	*/
	
	matView_ = math::make_look_at(cam.eye(), cam.center(), cam.up());

	if( keyboard_.isKeyDown(djah::system::input::eKC_SHIFT)
		|| gamepad_.isPlugged() && gamepad_.getButton(system::input::eX360_RB).isDown() )
	{
		spotDirection_ = (cam.center() - cam.eye()).normalize();
		spotPosition_  = cam.eye() - 0.2f * cam.up() + spotDirection_ - cam.up().cross(spotDirection_);
		matSpotView_   = math::make_look_at(spotPosition_, spotPosition_ + spotDirection_, cam.up());
	}
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void ShadowTest::draw()
{
	pFBO_->bind();
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDrawBuffer(GL_NONE);
	glViewport(0, 0, SH_VP_W, SH_VP_H);
	//glCullFace(GL_BACK);
	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&matShadowPerspectiveProj_[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&matSpotView_[0][0]);

	pVertexArray_->draw();

	glPushMatrix();
	glMultMatrixf(&cow_->mat_world_[0][0]);
	cow_->va_->draw();
	glPopMatrix();

	glPushMatrix();
	glMultMatrixf(&astroboy_->mat_world_[0][0]);
	astroboy_->va_->draw();
	glPopMatrix();

	glPushMatrix();
	glMultMatrixf(&cthulhu_->mat_world_[0][0]);
	cthulhu_->va_->draw();
	glPopMatrix();

	drawAxis(false);


	opengl::frame_buffer::bind_default_frame_buffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, pDevice_->videoConfig().width, pDevice_->videoConfig().height);
	//glCullFace(GL_BACK);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&matPerspectiveProj_[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&matView_[0][0]);

	drawAxis(true);

	drawMeshes();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void ShadowTest::drawAxis(bool shadowMap)
{
	static const float c = 0.8f;

	if( shadowMap )
	{
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		pShadowMap_->bind();

		glBegin(GL_QUADS);
		{
			glColor3f(c,c,c);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(10.0f, 0.0f, -5.0f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(10.0f, 10.0f, -5.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(10.0f, 10.0f, 5.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(10.0f, 0.0f, 5.0f);
		}
		glEnd();

		pShadowMap_->unbind();
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
		glBegin(GL_LINES);
		glColor3f(1,1,0);
		glVertex3fv(spotPosition_.data);
		glVertex3fv((spotPosition_ + spotDirection_ * 50.0f).data);
		glEnd();
		//glEnable(GL_CULL_FACE);
		glPopMatrix();
	}

	glBegin(GL_LINES);
	{
		//X
		glColor3fv(math::vector3f::x_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::x_axis.data);
		//Y
		glColor3fv(math::vector3f::y_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::y_axis.data);
		//Z
		glColor3fv(math::vector3f::z_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::z_axis.data);
	}
	glEnd();
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void ShadowTest::drawMeshes()
{
	opengl::texture::set_active_unit(1);
	pShadowMap_->bind();

	const math::matrix4f lightVP = matSpotView_ * matShadowPerspectiveProj_;
	{
		cow_->sp_->begin();


		opengl::texture::set_active_unit(0);
		pFloorTexture_->bind();

		cow_->sp_->sendUniform("in_DiffuseSampler", 0);
		cow_->sp_->sendUniform("in_ShadowMapSampler", 1);
		cow_->sp_->sendUniform("in_SpotPosition", spotPosition_);
		cow_->sp_->sendUniform("in_SpotDirection", spotDirection_);

		cow_->sp_->sendUniformMatrix("in_WorldViewProjection", matView_ * matPerspectiveProj_);
		cow_->sp_->sendUniformMatrix("in_World", math::matrix4f::identity);
		cow_->sp_->sendUniformMatrix("in_LightWVP", lightVP);
		pVertexArray_->draw();
		cow_->sp_->end();

		pFloorTexture_->unbind();
	}

	astroboy_->draw(matPerspectiveProj_, matView_, lightVP, spotPosition_, spotDirection_);

	cow_->trans_.rotate(math::make_quaternion(math::deg_to_rad(0.5f), math::vector3f::y_axis));

	cow_->draw(matPerspectiveProj_, matView_, lightVP, spotPosition_, spotDirection_);

	cthulhu_->draw(matPerspectiveProj_, matView_, lightVP, spotPosition_, spotDirection_);

	opengl::texture::set_active_unit(1);
	pShadowMap_->unbind();
	opengl::texture::set_active_unit(0);
}
//--------------------------------------------------------------------------------------------------