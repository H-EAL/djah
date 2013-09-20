#include <iostream>

#include <djah/debug/assertion.hpp>
#include <djah/debug/xml_sink.hpp>
#include <djah/debug/console_sink.hpp>
#include <djah/debug/core_logger.hpp>

#include <djah/system/device.hpp>
#include <djah/system/driver.hpp>

#include <djah/system/input/keyboard.hpp>
#include <djah/system/input/gamepad.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>

#include <djah/resources/asset_finder.hpp>
#include <djah/resources/config_object.hpp>

#include <djah/gameplay/game_object.hpp>
#include <djah/gameplay/components.hpp>

#include <djah/3d/shader.hpp>

#include <djah/math.hpp>

#include "resource_finder.hpp"

using namespace djah;

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

void initLoggers()
{
	debug::sink_ptr pOpenGLSink    ( new debug::xml_sink("log/opengl.log.xml"    , debug::log_filter("opengl")) );
	debug::sink_ptr pSystemSink    ( new debug::xml_sink("log/system.log.xml"    , debug::log_filter("system")) );
	debug::sink_ptr pFileSystemSink( new debug::xml_sink("log/filesystem.log.xml", debug::log_filter("fs")) );
	debug::sink_ptr pGlobalSink    ( new debug::xml_sink("log/global.log.xml"    , debug::log_filter("any")) );

	debug::sink_ptr pConsoleSink   ( new debug::console_sink     ( debug::log_filter("any") ) );
	debug::sink_ptr pDebugSink     ( new debug::output_debug_sink( debug::log_filter("any")                 ) );


	debug::core_logger::get().addSink( pOpenGLSink );
	debug::core_logger::get().addSink( pSystemSink );
	debug::core_logger::get().addSink( pFileSystemSink );
	debug::core_logger::get().addSink( pGlobalSink );

	debug::core_logger::get().addSink( pConsoleSink );
	debug::core_logger::get().addSink( pDebugSink );

	std::cout << djahLogo() << std::endl;
}

class device_cfg
	: public resources::config_object<system::device_config>
{
public:
	device_cfg(const std::string &url)
		: resources::config_object<system::device_config>(url)
	{
		list_config_vars
		(
			width,
			height,
			colorBits,
			depthBits,
			stencilBits,
			fullscreen,
			title
		);
	}
};

class driver_cfg
	: public resources::config_object<system::driver_config>
{
public:
	driver_cfg(const std::string &url)
		: resources::config_object<system::driver_config>(url)
	{
		list_config_vars
		(
			majorVersion,
			minorVersion,
			isOpenglES,
			enableDebug,
			enableCompatibilityProfile,
			vsync
		);
	}
};

typedef gameplay::game_object<gameplay::components::DefaultComponentTypes> game_object_t;

namespace djah { namespace opengl { template<typename T> class uniform {}; } }

int main()
{
	initLoggers();

	// 0 - Init system
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./config"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./assets"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./assets/meshes"));

	resources::asset_finder<> assetFinder;
	assetFinder.registerLoader(std::make_shared<resources::loader<resources::mesh>>(), "mesh");
	assetFinder.registerLoader(std::make_shared<resources::loader<resources::data_object<>>>(), "config");

	game_object_t camera("cam");
	camera.use<gameplay::components::transform>();
	camera.use<gameplay::components::fov>();


	// 1 - Create device = create window
	auto pDeviceConfig = resources::open_config<device_cfg>("device.config");
	DJAH_GLOBAL_NOTIFICATION() << "Device configuration:\n" << pDeviceConfig->toString() << DJAH_END_LOG();
	system::device_sptr pDevice = system::create_device(pDeviceConfig);
	check(pDevice);

	// 2 - Create a driver linked to the previously created device
	auto pDriverConfig = resources::open_config<driver_cfg>("driver.config");
	DJAH_GLOBAL_NOTIFICATION() << "Driver configuration:\n" << pDriverConfig->toString() << DJAH_END_LOG();
	system::driver_sptr pDriver = pDevice->createDriver(pDriverConfig);
	check(pDriver);


	// 3 - Init renderer
	glClearColor(0,0,0, 1.0f);
	glEnable(GL_DEPTH_TEST);
	system::input::keyboard kb;
	system::input::gamepad gp(0);
	bool fs = pDeviceConfig->fullscreen;
	bool resolutionChanged = true;

	// 3.5 - Load resources
	d3d::shader simpleShader("lit_textured");
	auto pMesh = assetFinder.get<resources::mesh>("feisar.mesh");
	auto pTexture = d3d::texture_manager::get().find("feisar-diffuse.png");
	
	math::matrix4f w = math::matrix4f::identity;
	math::matrix4f v = math::make_look_at(math::vector3f(0,5,-15), math::vector3f(0,1,60), math::vector3f::y_axis);
	math::matrix4f p = math::matrix4f::identity;
	math::matrix4f wvp = math::matrix4f::identity;
	
	//opengl::uniform<math::matrix4f> in_World("in_World", simpleShader.program());
	//in_World = math::matrix4f::identity;

	pMesh->init(simpleShader.program());
	pTexture->bind();
	simpleShader.program().begin();
	simpleShader.program().sendUniform("in_World", math::matrix4f::identity);
	simpleShader.program().sendUniform("in_DiffuseSampler",0);
	simpleShader.program().end();

	float currentAngle = 0.0f;
	float targetAngle = 0.0f;
	float maxAngle = 60.0f;

	// 4 - Run app
	while( pDevice->run() )
	{
		kb.update();
		gp.update();	

		if( kb.pressed(system::input::eKC_ESCAPE) )
		{
			pDevice->shutDown();
		}

		if( kb.pressed(system::input::eKC_F) )
		{
			fs = !fs;
			pDevice->setResolution(fs);
		}

		if( kb.pressed(system::input::eKC_F1) )
		{
			resolutionChanged = pDevice->setResolution(800,600);
		}

		if( kb.pressed(system::input::eKC_F2) )
		{
			resolutionChanged = pDevice->setResolution(1024,768);
		}

		if( kb.pressed(system::input::eKC_F3) )
		{
			resolutionChanged = pDevice->setResolution(1280,720);
		}

		if( kb.pressed(system::input::eKC_F4) )
		{
			resolutionChanged = pDevice->setResolution(480,640);
		}

		if( resolutionChanged )
		{
			p = math::make_perspective_projection(60.0f, (float)pDeviceConfig->width/pDeviceConfig->height, 0.01f, 1000.0f);
			glViewport(0,0,pDeviceConfig->width, pDeviceConfig->height);
			resolutionChanged = false;
		}

		if( gp.isPlugged() )
		{
			const float leftEngine = gp.getTrigger(system::input::eX360_LeftTrigger).value();
			const float rightEngine = gp.getTrigger(system::input::eX360_RightTrigger).value();
			targetAngle = (rightEngine - leftEngine) * maxAngle;

			gp.vibrate(rightEngine, leftEngine);

			if( targetAngle == 0.0f )
			{
				currentAngle = math::rotate(currentAngle, 0.0f, 360.0f);
				if( targetAngle > 180.0f )
				{
					targetAngle -= 180.0f;
				}
			}
		}

		static const float angleSpeed = 5.0f;
		if( currentAngle < targetAngle )
		{
			currentAngle = math::clamp(currentAngle + angleSpeed, -maxAngle, targetAngle);
		}
		else if(currentAngle > targetAngle )
		{
			currentAngle = math::clamp(currentAngle - angleSpeed, targetAngle, maxAngle);
		}

		w = math::make_rotation(math::deg_to_rad(currentAngle), math::vector3f::z_axis);
		wvp = w*v*p;

		pDriver->beginFrame();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		simpleShader.program().begin();
		simpleShader.program().sendUniform("in_WVP", wvp);
		pMesh->draw();
		simpleShader.program().end();

		pDriver->endFrame();
	}

	return EXIT_SUCCESS;
}