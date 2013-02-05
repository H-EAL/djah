#include <iostream>

#include <djah/system/device.hpp>

#include <djah/system/input/keyboard.hpp>
#include <djah/system/input/mouse.hpp>
#include <djah/system/input/gamepad.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>

#include <djah/debug/log.hpp>
#include <djah/debug/console_sink.hpp>

#include <djah/core/time/timer.hpp>

#include "grid.hpp"

using namespace djah;

class input_manager
{
public:
	static void update()
	{
		mouse_.update();
		gamepad_.update();
		keyboard_.update();
	}

public:
	static system::input::mouse		mouse_;
	static system::input::gamepad	gamepad_;
	static system::input::keyboard	keyboard_;
};

system::input::mouse	input_manager::mouse_;
system::input::gamepad	input_manager::gamepad_(0);
system::input::keyboard input_manager::keyboard_;

void initLoggers()
{
	debug::sink_ptr pConsoleSink   ( new debug::console_sink     ( debug::log_filter("any", debug::warning) ) );
	debug::sink_ptr pDebugSink     ( new debug::output_debug_sink( debug::log_filter("any")                 ) );

	debug::core_logger::get().addSink( pConsoleSink );
	debug::core_logger::get().addSink( pDebugSink );
}

int main()
{
	initLoggers();

	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("."));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data/data_objects"));

	system::device_ptr pDevice = nullptr;
	system::driver_ptr pDriver = nullptr;

	pDevice = system::create_device(1280, 800);
	pDriver = pDevice ? pDevice->videoDriver() : nullptr;

	if( !pDevice || !pDriver )
		return EXIT_FAILURE;

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glCullFace(GL_BACK);

	grid g(100,100);

	while( pDevice->run() )
	{
		static time::timer dtClk;
		const float dt = dtClk.getElapsedTimeSec();
		dtClk.restart();

		input_manager::update();

		static math::vector3f eye(0,5,0);
		static float speed = 1.0f;
		static float fovy = 60.0f;
		if( pDevice->hasWindowFocus() )
		{
			if( input_manager::keyboard_.isKeyDown(system::input::eKC_ESCAPE) )
				pDevice->shutDown();

			/*
			math::vector3f m;
			if( input_manager::keyboard_.isKeyDown(system::input::eKC_LEFT) )
				m.x -= 1.0f;
			if( input_manager::keyboard_.isKeyDown(system::input::eKC_RIGHT) )
				m.x += 1.0f;
			if( input_manager::keyboard_.isKeyDown(system::input::eKC_UP) )
				m.z += 1.0f;
			if( input_manager::keyboard_.isKeyDown(system::input::eKC_DOWN) )
				m.z -= 1.0f;

			if( m.lengthSq() > 0.0f )
				m.normalize();

			if( input_manager::keyboard_.isKeyDown(system::input::eKC_F1) )
				speed = math::clamp(speed - 0.01f, 0.1f, 10.0f);
			if( input_manager::keyboard_.isKeyDown(system::input::eKC_F2) )
				speed = math::clamp(speed + 0.01f, 0.1f, 10.0f);

			eye += m * speed * dt;*/

			static bool isDragging = false;
			if( input_manager::mouse_.leftButton().isDown() )
			{
				if( isDragging )
				{
					eye.x += float(input_manager::mouse_.delta().x) * 0.005f;
					eye.z += float(input_manager::mouse_.delta().y) * 0.005f;
				}
				else
				{
					isDragging = true;
				}
			}
			else
			{
				isDragging = false;
			}


			if( input_manager::keyboard_.pressed(system::input::eKC_F11) )
				fovy = math::clamp(fovy - 5.0f, 5.0f, 175.0f);
			if( input_manager::keyboard_.pressed(system::input::eKC_F12) )
				fovy = math::clamp(fovy + 5.0f, 5.0f, 175.0f);

			if( input_manager::mouse_.rightButton().pressed() )
			{
				std::cout << g.cellAt(input_manager::mouse_.position()) << std::endl;
				g.destroyCell(input_manager::mouse_.position());
			}

			if( input_manager::mouse_.delta().lengthSq() > 0.0f )
			{
				g.highlightCell(input_manager::mouse_.position());
			}
		}

		const math::matrix4f &matProj = math::make_perspective_projection(fovy, 1280.0f/800.0f, 0.1f, 100.0f);
		const math::vector3f &center = eye + math::vector3f(0,-1,0);

		const math::matrix4f &matView = math::make_look_at(eye, center, math::vector3f(0,0,1));
		const math::matrix4f &matVP = matView * matProj;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		g.draw(matVP, eye);

		pDevice->swapBuffers();
	}


	return EXIT_SUCCESS;
}