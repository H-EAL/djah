#include "application.hpp"

#include <djah/opengl.hpp>
#include <djah/3d/text.hpp>
#include <djah/3d/font_engine.hpp>

#include <djah/system/video_config.hpp>
#include <djah/system/device.hpp>
#include <djah/system/driver.hpp>

#include <djah/debug/log.hpp>
#include <djah/debug/core_logger.hpp>
#include <djah/debug/basic_logger.hpp>
#include <djah/debug/xml_sink.hpp>
#include <djah/debug/console_sink.hpp>

#include <djah/core/time/timer.hpp>

#include <djah/core/murmur_hash.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>
#include <djah/filesystem/memory_stream.hpp>
#include <djah/filesystem/url.hpp>

#include <djah/resources/asset_finder.hpp>
#include <djah/resources/asset_warehouse.hpp>
#include <djah/resources/image_loader.hpp>

#include <djah/dataobject/registry.hpp>

#include "test.hpp"
#include "model.hpp"
#include "profiler.hpp"

#define SH_VP_W 1 << 10
#define SH_VP_H 1 << 10

int ScopedProfile::i = 0;


const char* djahLogo()
{
	return
	"              .___    __       .__     \n"
	"            __| _/   |__|____  |  |__  \n"
	"           / __ |    |  \\__  \\ |  |  \\ \n"
	"          / /_/ |    |  |/ __ \\|   |  \\\n"
	"          \\____ |/\\__|  (____  /___|  /\n"
	"               \\/\\______|    \\/     \\/ \n"
	"          							  ";
}

void printInfosAux()
{
	std::string extensions;
	std::for_each
	(
		opengl::capabilities::extensions_.begin(),
		opengl::capabilities::extensions_.end(),
		[&extensions](const std::string &ext) { extensions += "| " + ext + "\n"; }
	);

	DJAH_BEGIN_LOG(notification)
		<< "\n"
		<< "===========================================================================\n"
		<< "| Renderer                 | " << opengl::capabilities::renderer()        << "\n"
		<< "| Vendor                   | " << opengl::capabilities::vendor()          << "\n"
		<< "| OpenGL version           | " << opengl::capabilities::opengl_version()  << "\n"
		<< "| GLSL version             | " << opengl::capabilities::glsl_version()    << "\n"
		<< "---------------------------------------------------------------------------\n"
		<< "| Available extensions (" << opengl::capabilities::extensions_.size()  << ")\n"
		<< "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
		<< extensions
		<< "==========================================================================="
		<< DJAH_END_LOG();
}

void APIENTRY oglDebugProc(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	void* userParam)
{
	switch( severity )
	{
	case GL_DEBUG_SEVERITY_LOW:
		DJAH_OPENGL_WARNING() << message << DJAH_END_LOG();
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		DJAH_OPENGL_ERROR() << message << DJAH_END_LOG();
		break;

	case GL_DEBUG_SEVERITY_HIGH:
		DJAH_ASSERT_MSG(false, "%s", message);
		break;
	}
}

void initLoggers()
{
	debug::sink_ptr pOpenGLSink    ( new debug::xml_sink("log/opengl.log.xml"    , debug::log_filter("opengl")) );
	debug::sink_ptr pSystemSink    ( new debug::xml_sink("log/system.log.xml"    , debug::log_filter("system")) );
	debug::sink_ptr pFileSystemSink( new debug::xml_sink("log/filesystem.log.xml", debug::log_filter("fs")) );
	debug::sink_ptr pGlobalSink    ( new debug::xml_sink("log/global.log.xml"    , debug::log_filter("any")) );

	debug::sink_ptr pConsoleSink   ( new debug::console_sink     ( debug::log_filter("any", debug::warning) ) );
	debug::sink_ptr pDebugSink     ( new debug::output_debug_sink( debug::log_filter("any")                 ) );


	debug::core_logger::get().addSink( pOpenGLSink );
	debug::core_logger::get().addSink( pSystemSink );
	debug::core_logger::get().addSink( pFileSystemSink );
	debug::core_logger::get().addSink( pGlobalSink );

	debug::core_logger::get().addSink( pConsoleSink );
	debug::core_logger::get().addSink( pDebugSink );

	std::cout << djahLogo() << std::endl;
}


application::application(int w, int h)
	: application_base(djah::system::device_config(w,h,32,24,0,false,true))
	, gamepad_(0)
	, pShadowTest_(nullptr)
	, pBasicTest_(nullptr)
	, pCurrentTest_(nullptr)
{
	initLoggers();

	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("."));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data/savegame"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data/data_objects"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data/game_objects"));

	filesystem::browser::get().addSavingChannel(new filesystem::directory_source("./data/savegame"));
	
	screenCfgDo_ = dataobject::default_registry::get().getDO("screenconfig");

	if( screenCfgDo_ )
	{
		application_base::video_config_.width		= screenCfgDo_->get<int>("width"		, w		);
		application_base::video_config_.height		= screenCfgDo_->get<int>("height"		, h		);
		application_base::video_config_.colorBits	= screenCfgDo_->get<int>("colorBits"	, 32	);
		application_base::video_config_.depthBits	= screenCfgDo_->get<int>("depthBits"	, 24	);
		application_base::video_config_.stencilBits	= screenCfgDo_->get<int>("stencilBits"	, 0		);
		application_base::video_config_.fullscreen	= screenCfgDo_->get<bool>("fullscreen"	, false	);
		application_base::video_config_.vsync		= screenCfgDo_->get<bool>("vsync"		, true	);

		cam.move( screenCfgDo_->get<math::vector3f>("cam_pos") );
	}
}

void application::initImpl()
{
	glDebugMessageCallbackARB(oglDebugProc, nullptr);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	printInfosAux();

	glClearDepth(1.f);
	glClearColor(0.0f, 0.0f, 0.1f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const float w = static_cast<float>(device_->config().width);
	const float h = static_cast<float>(device_->config().height);
	matOrthoProj_ = math::make_orthographic_projection(0.0f, w, h, 0.0f, -1.0f, 1.0f);

	d3d::font_engine::create();
	d3d::font_engine::get().setFontsPath("fonts/");
	//fps_str_.draw();

	fps_str_.setPosition(math::vector2i(0,0));
	mouse_pos_.setPosition(math::vector2i(0,15));
	cam_pos_.setPosition(math::vector2i(0,30));
	test_name_.setPosition(math::vector2i(device_->config().width/2,0));

	pShadowTest_			= new ShadowTest(device_, mouse_, keyboard_, gamepad_, cam);
	pBasicTest_				= new BasicTest(device_, cam);
	pDeferredShadingTest_	= new DeferredShadingTest(device_, gamepad_, cam);
	pBumpMappingTest_		= new BumpMappingTest(device_, gamepad_, cam);
	pFontTest_				= new FontTest(device_, cam);
	pSolarSystemTest_		= new SolarSystemTest(device_, gamepad_, cam);
	pTessTest_				= new TesselationTest(device_, keyboard_, cam);

	resources::asset_warehouse::get().cleanUp();

	pCurrentTest_ = pBasicTest_;
	pCurrentTest_->onInit();
}

void application::runImpl()
{
	static time::timer dtClk;
	const float dt = dtClk.getElapsedTimeSec();
	dtClk.restart();

	static int fps = 0;
	static time::timer clk;
	if( clk.getElapsedTimeMs() < 1000.0f )
		++fps;
	else
	{
		std::stringstream ss;
		ss << fps << " FPS";
		fps_str_ = ss.str();
		clk.restart();
		fps = 1;
	}

	mouse_.update();
	keyboard_.update();
	gamepad_.update();

	if( keyboard_.released(system::input::eKC_ESCAPE) )
		device_->shutDown();

	djah::math::vector3f m;
	float rx = 0.0f;
	float ry = 0.0f;

	if( gamepad_.isPlugged() )
	{
		if( gamepad_.getAxis(system::input::eX360_LeftRightTrigger).correctedValue() > 0.5f )
			m.y = -0.1f;
		if( gamepad_.getAxis(system::input::eX360_LeftRightTrigger).correctedValue() < -0.5f )
			m.y = 0.1f;

		m.x = gamepad_.getAxis(system::input::eX360_LeftX).correctedValue(0.2f);
		m.z = gamepad_.getAxis(system::input::eX360_LeftY).correctedValue(0.1f);
		rx = gamepad_.getAxis(system::input::eX360_RightX).correctedValue(0.1f) * 2.5f;
		ry = gamepad_.getAxis(system::input::eX360_RightY).correctedValue(0.1f) * 2.5f;

		int l = 0, r =0;
		if( gamepad_.getButton(system::input::eX360_LB).isDown() )
		{
			l = 65535;
		}
		if( gamepad_.getButton(system::input::eX360_RB).isDown() )
		{
			r = 65535;
		}
		gamepad_.vibrate(l,r);
	}

	if( device_->hasFocus() )
	{
		if( keyboard_.isKeyDown(djah::system::input::eKC_A) )
			m.x -= 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::eKC_D) )
			m.x += 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::eKC_Q) )
			m.y -= 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::eKC_E) )
			m.y += 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::eKC_W) )
			m.z -= 1.0f;
		if( keyboard_.isKeyDown(djah::system::input::eKC_S) )
			m.z += 1.0f;

		if( mouse_.leftButton().isDown() )
		{
			rx = static_cast<float>(mouse_.delta().y) * 0.7f;
			ry = static_cast<float>(mouse_.delta().x) * 0.7f;
		}

		std::stringstream ss;
		ss << "Mouse position: " << mouse_.position();
		mouse_pos_ = ss.str();

		test_base *previousTest = pCurrentTest_;

			 if( keyboard_.pressed(djah::system::input::eKC_1) )
			pCurrentTest_ = pBasicTest_;
		else if( keyboard_.pressed(djah::system::input::eKC_2) )
			pCurrentTest_ = pShadowTest_;
		else if( keyboard_.pressed(djah::system::input::eKC_3) )
			pCurrentTest_ = pDeferredShadingTest_;
		else if( keyboard_.pressed(djah::system::input::eKC_4) )
			pCurrentTest_ = pSolarSystemTest_;
		else if( keyboard_.pressed(djah::system::input::eKC_5) )
			pCurrentTest_ = pBumpMappingTest_;
		else if( keyboard_.pressed(djah::system::input::eKC_6) )
			pCurrentTest_ = pFontTest_;
		else if( keyboard_.pressed(djah::system::input::eKC_7) )
			pCurrentTest_ = pTessTest_;

		if( pCurrentTest_ != previousTest )
		{
			DJAH_GLOBAL_NOTIFICATION() << "Exit from test: \"" << previousTest->name() << "\"" << DJAH_END_LOG();
			previousTest->onExit();
			DJAH_GLOBAL_NOTIFICATION() << "Entering in test: \"" << pCurrentTest_->name() << "\"" << DJAH_END_LOG();
			pCurrentTest_->onInit();
		}
	}

	if( m.lengthSq() > 0.0f )
	{
		const float speed = screenCfgDo_->get<float>("cam_speed", 0.5f);
		m.normalize() *= speed;
		cam.move(m);
	}
	cam.rotatex	(rx);
	cam.rotatey	(ry);

	std::stringstream ss;
	ss << "Camera position: " << cam.eye();
	cam_pos_ = ss.str();

	test_name_ = pCurrentTest_->name();

	pCurrentTest_->update(dt);
	pCurrentTest_->draw();

	draw2D();
}

void application::exitImpl()
{
	pCurrentTest_ = nullptr;

	delete pShadowTest_;
	delete pBasicTest_;
	delete pDeferredShadingTest_;
	delete pSolarSystemTest_;
	delete pBumpMappingTest_;
	delete pTessTest_;
	delete pFontTest_;

	d3d::font_engine::destroy();
}

void application::draw2D()
{
	//return;
	opengl::frame_buffer::bind_default_frame_buffer();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&matOrthoProj_[0][0]);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	fps_str_.draw();
	mouse_pos_.draw();
	cam_pos_.draw();
	test_name_.draw();
	glDisable(GL_BLEND);
}