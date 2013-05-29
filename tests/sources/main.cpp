#include <iostream>

#include <djah/debug/assertion.hpp>

#include <djah/system/device.hpp>
#include <djah/system/driver.hpp>
#include <djah/system/input/keyboard.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>

#include <djah/3d/shader.hpp>

#include <djah/math.hpp>

#include "resource_finder.hpp"

using namespace djah;


int main()
{
	// 0 - Init system
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./config"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data/meshes"));

	// 1 - Create device = create window
	system::device_config_sptr pDeviceConfig = system::open_config<system::device_config>("device.config");
	system::device_sptr pDevice = system::create_device(pDeviceConfig);
	check(pDevice);

	// 2 - Create a driver linked to the previously created device
	system::driver_config_sptr pDriverConfig = system::open_config<system::driver_config>("driver.config");
	system::driver_sptr pDriver = system::create_driver(pDevice, pDriverConfig);
	check(pDriver);

	// 3 - Init renderer
	glClearColor(0,1,0, 1.0f);
	glEnable(GL_DEPTH_TEST);
	system::input::keyboard kb;
	bool fs = pDeviceConfig->fullscreen;

	// 3.5 - Load resources
	d3d::shader simpleShader("lit_textured");
	auto pMesh = find_resource<resources::mesh>("feisar.mesh");
	auto pTexture = d3d::texture_manager::get().find("feisar-diffuse.png");
	
	math::matrix4f wvp =
		  math::make_look_at(math::vector3f(0,5,-10), math::vector3f(0,1,60), math::vector3f::y_axis)
		* math::make_perspective_projection(90.0f, 192.0f/108.0f, 0.01f, 1000.0f);
	
	pMesh->init(simpleShader.program());
	pTexture->bind();
	simpleShader.program().begin();
	simpleShader.program().sendUniform("in_World", math::matrix4f::identity);
	simpleShader.program().sendUniform("in_WVP", wvp);
	simpleShader.program().sendUniform("in_DiffuseSampler",0);
	simpleShader.program().end();

	// 4 - Run app
	while( pDevice->run() )
	{
		kb.update();

		if( kb.pressed(system::input::eKC_ESCAPE) )
		{
			pDevice->shutDown();
		}

		if( kb.pressed(system::input::eKC_F) )
		{
			fs = !fs;
			pDevice->toggleFullScreen(fs);
		}

		if( kb.pressed(system::input::eKC_F1) )
		{
			pDeviceConfig->width = 800;
			pDeviceConfig->height = 600;
			pDevice->toggleFullScreen(true);
			glViewport(0,0,pDeviceConfig->width, pDeviceConfig->height);
		}

		if( kb.pressed(system::input::eKC_F2) )
		{
			pDeviceConfig->width = 1024;
			pDeviceConfig->height = 768;
			pDevice->toggleFullScreen(true);
			glViewport(0,0,pDeviceConfig->width, pDeviceConfig->height);
		}

		if( kb.pressed(system::input::eKC_F3) )
		{
			pDeviceConfig->width = 1280;
			pDeviceConfig->height = 720;
			pDevice->toggleFullScreen(true);
			glViewport(0,0,pDeviceConfig->width, pDeviceConfig->height);
		}

		if( kb.pressed(system::input::eKC_F4) )
		{
			pDeviceConfig->width = 1920;
			pDeviceConfig->height = 1080;
			pDevice->toggleFullScreen(true);
			glViewport(0,0,pDeviceConfig->width, pDeviceConfig->height);
		}

		pDriver->beginFrame();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		simpleShader.program().begin();
		pMesh->draw();
		simpleShader.program().end();

		pDriver->endFrame();
	}

	return EXIT_SUCCESS;
}