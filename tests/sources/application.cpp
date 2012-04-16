#include "application.hpp"

#include <djah/video/ogl.hpp>
#include <djah/video/text.hpp>
#include <djah/video/font_engine.hpp>

#include <djah/system/video_config.hpp>
#include <djah/system/device.hpp>
#include <djah/system/driver_base.hpp>

#include <djah/log/logger.hpp>
#include <djah/log/console_logger.hpp>

#include <djah/time/timer.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>
#include <djah/filesystem/memory_stream.hpp>
#include <djah/filesystem/url.hpp>

#include <djah/resources/media_manager.hpp>
#include <djah/resources/resource_manager.hpp>
#include <djah/resources/image_loader.hpp>

#include "test.hpp"
#include "model.hpp"
#include "profiler.hpp"

using namespace djah;


application::application(int w, int h)
	: application_base(djah::system::video_config(w,h,32,24,0,false,true))
	, eye_(0,11.8f,math::pi_over_2)
	, center_(0,0,0)
	, up_(0,1,0)
	, gamepad_(0)
{
}

void application::initImpl()
{
	log::logger::setLogger(new log::console_logger);
	//testURL();
	testMat();
		
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("."));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data"));

	const float w = static_cast<float>(device_->videoConfig().width);
	const float h = static_cast<float>(device_->videoConfig().height);
	matPerspectiveProj_ = math::make_perspective_projection(60.0f, w/h, 0.1f, 1000.f);
	matOrthoProj_ = math::make_orthographic_projection(0.0f, w, h, 0.0f, -1.0f, 1.0f);
		
	glClearDepth(1.f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	video::font_engine::create();
	video::font_engine::get().setFontsPath("fonts/");

	astroboy_ = new mesh("astroboy", "boy_10.tga", "", 2);
	cthulhu_ = new mesh("cthulhu_1", "cthulhu.jpg", /*"cthulhu_normalmap.jpg"*/"", 2);
	cow_ = new mesh("cow", "cow.jpg", "", 2);
	dude_ = new mesh("dude", "", "", 3);

	mouse_pos_.setPosition(math::vector2i(100,200));
}

void application::runImpl()
{
	static int fps = 0;
	static time::timer clk;

	mouse_.update();
	keyboard_.update();
	gamepad_.update();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	djah::math::vector3f m;
	float rx = 0.0f;
	float ry = 0.0f;

	if( gamepad_.isPlugged() )
	{
		if( gamepad_.getButton(6).isDown() && gamepad_.getButton(7).isDown() )
			m.y = -0.1f;
		if( gamepad_.getButton(4).isDown() && gamepad_.getButton(5).isDown() )
			m.y = 0.1f;

		m.x = gamepad_.getAxis(0).scaledValue(0.1f) * 0.5f;
		m.z = gamepad_.getAxis(1).scaledValue(0.1f) * 0.5f;
		rx = gamepad_.getAxis(3).scaledValue(0.1f) * 1.5f;
		ry = gamepad_.getAxis(2).scaledValue(0.1f) * 1.5f;
	}
	else
	{
		if( keyboard_.isKeyDown(djah::system::input::EKC_W) )
			m.z -= 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::EKC_S) )
			m.z += 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::EKC_A) )
			m.x -= 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::EKC_D) )
			m.x += 1.0f;

		if( mouse_.leftButton().isDown() )
		{
			rx = static_cast<float>(mouse_.delta().y) * 0.5f;
			ry = static_cast<float>(mouse_.delta().x) * 0.5f;
		}
	}

	if( m.lengthSq() > 0.0f )
	{
		const float speed = 0.5f;
		m.normalize() *= speed;
	}
	cam.movex	(m.x);
	cam.movey	(m.y);
	cam.movez	(m.z);
	cam.rotatex	(rx);
	cam.rotatey	(ry);
	
	matView_ = math::make_look_at(cam.eye(), cam.center(), cam.up());

	/**/
	draw3D();
	/**/
	draw2D();

	if( clk.getElapsedTimeMs() < 1000 )
		++fps;
	else
	{
		std::stringstream ss;
		ss << fps+1 << " FPS";
		fps_str_ = ss.str();
		clk.restart();
		fps = 0;
	}
	
	if( device_->hasWindowFocus() )
	{
		std::stringstream ss;
		ss << "Mouse | " << mouse_.leftButton().isDown() << " | " << mouse_.middleButton().isDown() << " | " << mouse_.rightButton().isDown() << " |";
		mouse_pos_ = ss.str();
	}
	else
	{
		mouse_pos_.clear();
	}
}

void application::exitImpl()
{
	video::font_engine::destroy();

	delete cthulhu_;
	delete astroboy_;
	delete cow_;
	delete dude_;
}

void application::draw3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(matPerspectiveProj_.getTransposed().data);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(matView_.getTransposed().data);

	drawAxis();
	drawMeshes();
}

void application::draw2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(matOrthoProj_.getTransposed().data);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	fps_str_.draw();
	mouse_pos_.draw();
}

void application::drawAxis()
{
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

	static const float s = 500.0f;
	static const float c = 0.8f;
	static const float h = -0.1f;
	glScalef(s, 1.0f, s);
	glBegin(GL_QUADS);
	{
		glColor3f(c,c,c);
		glVertex3f(-0.5f, h, -0.5f);
		glVertex3f(-0.5f, h,  0.5f);
		glVertex3f( 0.5f, h,  0.5f);
		glVertex3f( 0.5f, h, -0.5f);
	}
	glEnd();
}

void application::drawMeshes()
{
	static const math::transformation_f transCow
	(
		math::vector3f(-20.0f, 0.0f, 0.0f),
		math::make_quaternion(math::deg_to_rad(-90.0f), math::vector3f::y_axis) * math::make_quaternion(math::deg_to_rad(-90.0f), math::vector3f::x_axis),
		math::vector3f(1.0f, 1.0f, 1.0f)
	);

	static const math::transformation_f transAstro
	(
		math::vector3f(20.0f, 0.0f, 0.0f),
		math::make_quaternion(math::deg_to_rad(-90.0f), math::vector3f::x_axis),
		math::vector3f(1.0f, 1.0f, 1.0f)
	);

	static const math::transformation_f transCthulhu
	(
		math::vector3f(-10.0f, 0.0f, 0.0f),
		math::make_quaternion(math::deg_to_rad(-90.0f), math::vector3f::y_axis) * math::make_quaternion(math::deg_to_rad(-90.0f), math::vector3f::x_axis),
		math::vector3f(1.0f, 1.0f, 1.0f)
	);

	static const math::transformation_f transDude
	(
		math::vector3f(10.0f, 5.0f, 0.0f),
		math::make_quaternion(math::deg_to_rad(-90.0f), math::vector3f::x_axis),
		math::vector3f(0.02f, 0.02f, 0.02f)
	);

	static bool init = false;
	if( !init )
	{
		cthulhu_->mat_world_	= transCthulhu.toMatrix4();
		astroboy_->mat_world_	= transAstro.toMatrix4();
		cow_->mat_world_		= transCow.toMatrix4();
		dude_->mat_world_		= transDude.toMatrix4();
		init = true;
	}

	cthulhu_->draw(matPerspectiveProj_, matView_, cam.eye());

	astroboy_->draw(matPerspectiveProj_, matView_, cam.eye());
	
	cow_->draw(matPerspectiveProj_, matView_, cam.eye());

	dude_->draw(matPerspectiveProj_, matView_, cam.eye());
	DJAH_BEGIN_LOG(EWL_NOTIFICATION) << cam.eye() << DJAH_END_LOG();
}