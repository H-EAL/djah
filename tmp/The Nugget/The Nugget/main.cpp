#include <iostream>
#include <ctime>

#include <djah/system/device.hpp>

#include <djah/system/input/keyboard.hpp>
#include <djah/system/input/mouse.hpp>
#include <djah/system/input/gamepad.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>

#include <djah/debug/log.hpp>
#include <djah/debug/console_sink.hpp>

#include <djah/core/time/timer.hpp>
#include <djah/core/state_machine.hpp>

#include <djah/gameplay/game_service.hpp>

#include "grid.hpp"
#include "item.hpp"

using namespace djah;


struct dn_vec
{
	dn_vec() {}
	dn_vec(const math::dual_number<float> &_x, const math::dual_number<float> &_y) : x(_x), y(_y) {}

	dn_vec operator *(const math::dual_number<float> &s)
	{
		return dn_vec(x*s, y*s);
	}

	dn_vec operator -() const
	{
		return dn_vec(-x, -y);
	}

	dn_vec operator +(const dn_vec &rhs)
	{
		return dn_vec(x + rhs.x, y + rhs.y);
	}

	dn_vec operator -(const dn_vec &rhs)
	{
		return dn_vec(x - rhs.x, y - rhs.y);
	}

	math::dual_number<float> x;
	math::dual_number<float> y;
};

dn_vec operator *(const math::dual_number<float> &lhs, const dn_vec &rhs)
{
	return dn_vec(rhs.x*lhs, rhs.y*lhs);
}


//==================================================================================================
struct RenderingCompare
{
	bool operator() (game_object_t *lhs, game_object_t *rhs)
	{
		return lhs->get<gameplay::components::texture>().file < rhs->get<gameplay::components::texture>().file;
	}
};
//==================================================================================================
class RenderingService
	: public gameplay::game_service
	<
		gameplay::components::default_components_tl,
		TYPELIST
		(
			gameplay::components::static_mesh,
			gameplay::components::transform,
			gameplay::components::texture
		),
		true,
		RenderingCompare
	>
{
public:
	RenderingService(d3d::shader &_shader)
		: shader_(_shader)
	{

	}

	void setViewProjectionMatrix(const math::matrix4f &_matVP)
	{
		matVP_ = _matVP;
	}

	void init()
	{
		auto itEnd = game_objects_.impl_.end();
		for(auto it = game_objects_.impl_.begin(); it != itEnd; ++it)
		{
			game_object_ptr pGo = *it;
			resources::mesh_ptr pMesh = find_resource<resources::mesh>(pGo->get<gameplay::components::static_mesh>().file);
			if( pMesh )
			{
				pMesh->init(shader_.program());
			}

			d3d::texture_manager::get().find(pGo->get<gameplay::components::texture>().file);
		}
	}

	virtual void execute(float dt)
	{
		const unsigned int nbGos = game_objects_.size();
		if( nbGos == 0 )
		{
			return;
		}

		std::string boundTexture = "";
		d3d::texture_ptr pTexture = nullptr;

		shader_.program().begin();
		shader_.program().sendUniform("in_DiffuseSampler", 0);

		auto itEnd = game_objects_.impl_.end();
		for(auto it = game_objects_.impl_.begin(); it != itEnd; ++it)
		{
			game_object_ptr pGo = *it;
			const std::string &currentTexture = pGo->get<gameplay::components::texture>().file;

			if( currentTexture != boundTexture )
			{
				pTexture = d3d::texture_manager::get().find(currentTexture);
				pTexture->bind();
				boundTexture = currentTexture;
			}

			executeFor(*pGo, dt);
		}

		shader_.program().end();

		opengl::texture::unbind();
	}

private:
	virtual void executeFor(game_object_t &gameObject, float dt)
	{
		const math::vector3f &pos = gameObject.get<gameplay::components::transform>().position * 1.1f + math::vector3f(0.55f, 0.55f, 0.0f);
		const math::vector3f &rot = gameObject.get<gameplay::components::transform>().orientation;

		const math::matrix4f &matWorld =
			  math::make_rotation(rot.x, math::vector3f::x_axis)
			* math::make_rotation(rot.y, math::vector3f::y_axis)
			* math::make_rotation(rot.z, math::vector3f::z_axis)
			* math::make_translation(pos);

		shader_.program().sendUniform("in_World", matWorld);
		shader_.program().sendUniform("in_WVP", matWorld * matVP_);

		resources::mesh_ptr pMesh = find_resource<resources::mesh>(gameObject.get<gameplay::components::static_mesh>().file);
		pMesh->draw();
	}

private:
	d3d::shader &shader_;
	math::matrix4f matVP_;
};
//==================================================================================================


//==================================================================================================
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
//==================================================================================================


//==================================================================================================
void initLoggers()
{
	debug::sink_ptr pConsoleSink   ( new debug::console_sink     ( debug::log_filter("any", debug::warning) ) );
	debug::sink_ptr pDebugSink     ( new debug::output_debug_sink( debug::log_filter("any")                 ) );

	debug::core_logger::get().addSink( pConsoleSink );
	debug::core_logger::get().addSink( pDebugSink );
}
//==================================================================================================


//==================================================================================================
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
//==================================================================================================


//////////////////////////////////////////////////////////////////////////
enum MyStates
{
	STATE_INVALID = -1,
	STATE_MAIN,
	STATE_RED,
	STATE_GREEN,
	STATE_BLUE,
	STATE_YELLOW,
	STATES_COUNT
};

static const std::string STATES[] =
{
	"STATE_MAIN",
	"STATE_RED",
	"STATE_GREEN",
	"STATE_BLUE",
	"STATE_YELLOW",
};

struct MyFSM
	: djah::state_machine<MyStates, MyFSM>
{
	void Init()
	{
		// States
		AddState<STATE_MAIN, STATE_INVALID, STATE_RED>();
		{
			AddState<STATE_RED,		STATE_MAIN>();
			AddState<STATE_GREEN,	STATE_MAIN>();
			AddState<STATE_BLUE,	STATE_MAIN>();
			AddState<STATE_YELLOW,	STATE_MAIN>();
		}

		// Transitions
		AddTransition<STATE_RED,	STATE_GREEN>();
		AddTransition<STATE_RED,	STATE_BLUE>();
		AddTransition<STATE_RED,	STATE_YELLOW>();
		AddTransition<STATE_GREEN,	STATE_RED>();
		AddTransition<STATE_GREEN,	STATE_BLUE>();
		AddTransition<STATE_GREEN,	STATE_YELLOW>();
		AddTransition<STATE_BLUE,	STATE_RED>();
		AddTransition<STATE_BLUE,	STATE_GREEN>();
		AddTransition<STATE_BLUE,	STATE_YELLOW>();
		AddTransition<STATE_YELLOW,	STATE_RED>();
		AddTransition<STATE_YELLOW,	STATE_GREEN>();
		AddTransition<STATE_YELLOW,	STATE_BLUE>();
		
		pvStates_[STATE_MAIN]->enter();
	}

	template<MyStates TState>
	void enter()
	{
	}

	template<MyStates TState>
	void update()
	{
	}

	template<MyStates TState>
	void exit()
	{
	}

	template<MyStates FromState, MyStates ToState>
	bool test()
	{
	}

	template<MyStates FromState, MyStates ToState>
	void fire()
	{
	}
};

static djah::math::vector3f COLOR;

template<>
void MyFSM::enter<STATE_RED>()
{
	std::cout << "Entering State RED!~!!" << std::endl;
	COLOR = djah::math::vector3f::x_axis;
	glClearColor(COLOR.x, COLOR.y, COLOR.z, 1);
}

template<>
void MyFSM::enter<STATE_GREEN>()
{
	std::cout << "Entering State GREEN!~!!" << std::endl;
	COLOR = djah::math::vector3f::y_axis;
	glClearColor(COLOR.x, COLOR.y, COLOR.z, 1);
	input_manager::gamepad_.vibrate(64555);
}

template<>
void MyFSM::enter<STATE_BLUE>()
{
	std::cout << "Entering State BLUE!~!!" << std::endl;
	COLOR = djah::math::vector3f::z_axis;
	glClearColor(COLOR.x, COLOR.y, COLOR.z, 1);
	input_manager::gamepad_.vibrate(64555);
}

template<>
void MyFSM::enter<STATE_YELLOW>()
{
	std::cout << "Entering State YELLOW!~!!" << std::endl;
	COLOR = djah::math::vector3f(1,1,0);
	glClearColor(COLOR.x, COLOR.y, COLOR.z, 1);
	input_manager::gamepad_.vibrate(64555);
}

template<>
void MyFSM::update<STATE_YELLOW>()
{
	input_manager::gamepad_.vibrate(0);
	//static int i = 0;
	//if(++i < 1000)
	//	input_manager::gamepad_.vibrate(64555);
}

template<>
bool MyFSM::test<STATE_RED, STATE_GREEN>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_A).pressed();
}

template<>
bool MyFSM::test<STATE_RED, STATE_BLUE>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_X).pressed();
}

template<>
bool MyFSM::test<STATE_RED, STATE_YELLOW>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_Y).pressed();
}


template<>
bool MyFSM::test<STATE_GREEN, STATE_RED>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_B).pressed();
}

template<>
bool MyFSM::test<STATE_GREEN, STATE_BLUE>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_X).pressed();
}

template<>
bool MyFSM::test<STATE_GREEN, STATE_YELLOW>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_Y).pressed();
}


template<>
bool MyFSM::test<STATE_BLUE, STATE_RED>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_B).pressed();
}

template<>
bool MyFSM::test<STATE_BLUE, STATE_GREEN>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_A).pressed();
}

template<>
bool MyFSM::test<STATE_BLUE, STATE_YELLOW>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_Y).pressed();
}


template<>
bool MyFSM::test<STATE_YELLOW, STATE_RED>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_B).pressed();
}

template<>
bool MyFSM::test<STATE_YELLOW, STATE_GREEN>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_A).pressed();
}

template<>
bool MyFSM::test<STATE_YELLOW, STATE_BLUE>()
{
	return input_manager::gamepad_.getButton(djah::system::input::eX360_X).pressed();
}
//////////////////////////////////////////////////////////////////////////


int main()
{
	srand((unsigned)::time(nullptr));
	MyFSM obj;

	obj.Init();

	initLoggers();

	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("."));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data/meshes"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data/textures"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./data/game_objects"));

	system::device_ptr pDevice = nullptr;
	system::driver_ptr pDriver = nullptr;

	pDevice = system::create_device(1280, 800);
	pDriver = pDevice ? pDevice->videoDriver() : nullptr;

	if( !pDevice || !pDriver )
		return EXIT_FAILURE;

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_BACK);

	glDebugMessageCallbackARB(oglDebugProc, nullptr);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	Grid g2(3,3);
	djah::d3d::shader sh("lit_textured");

	std::vector<game_object_t*> items(2);
	items[0] = new Item("item0");
	items[1] = new Item("item1");
	game_object_t::serializer_t::deserialize(items);


	RenderingService rdrSrv(sh);
	rdrSrv.add(items[0]);
	rdrSrv.add(items[1]);
	rdrSrv.init();


	while( pDevice->run() )
	{
		static time::timer dtClk;
		const float dt = dtClk.getElapsedTimeSec();
		dtClk.restart();

		input_manager::update();
		//obj.Run<STATE_MAIN>();

		static math::vector3f eye(0,0,5);
		static float speed = 1.0f;
		static float fovy = 60.0f;
		if( pDevice->hasWindowFocus() )
		{
			if( input_manager::keyboard_.isKeyDown(system::input::eKC_ESCAPE) )
				pDevice->shutDown();


			static bool isDragging = false;

			if( input_manager::mouse_.leftButton().isDown() )
			{
				if( isDragging )
				{
					eye.x -= float(input_manager::mouse_.delta().x) * 0.005f;
					eye.y += float(input_manager::mouse_.delta().y) * 0.005f;
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

			if( !isDragging && input_manager::mouse_.rightButton().pressed() )
			{
				std::cout << g2.cellAt(input_manager::mouse_.position()) << std::endl;
				g2.destroyCell(input_manager::mouse_.position(), eye);
			}


			if( input_manager::keyboard_.pressed(system::input::eKC_F11) )
				fovy = math::clamp(fovy - 5.0f, 5.0f, 175.0f);
			if( input_manager::keyboard_.pressed(system::input::eKC_F12) )
				fovy = math::clamp(fovy + 5.0f, 5.0f, 175.0f);
			/*
			if( input_manager::mouse_.delta().lengthSq() > 0.0f )
			{
				g2.destroyCell(input_manager::mouse_.position(), eye);
			}
			*/

			if( input_manager::keyboard_.pressed(system::input::eKC_SPACE) )
			{
				std::cout << eye << std::endl;
			}
		}
		const math::matrix4f &matProj = math::make_perspective_projection(fovy, 1280.0f/800.0f, 0.1f, 100.0f);
		const math::vector3f &center = eye + math::vector3f(0,0,-1);

		const math::matrix4f &matView = math::make_look_at(eye, center, math::vector3f(0,1,0));
		const math::matrix4f &matVP = matView * matProj;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glDisable(GL_BLEND);
		g2.draw(eye, matVP);

		rdrSrv.setViewProjectionMatrix(matVP);
		rdrSrv.execute(0.0f);



		static const dn_vec p0(0.0f, 0.0f);
		static const dn_vec p1(1.0f, 1.0f);
		static const dn_vec p2(6.0f, 0.5f);
		static const dn_vec p3(3.0f, 1.8f);


		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(&matProj[0][0]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(&matView[0][0]);
		glBegin(GL_LINE_STRIP);
		
		for(float t = 0.0f; t <= 1.0f; t += 0.02f)
		{
			math::dual_number<float> dnt(t, 1.0f);
			const dn_vec &p = math::catmull_rom(dnt, p0, p0, p1, p2);
			math::vector2f n(p.x.dual(), p.y.dual());
			if( n.lengthSq() > 0.0f )
				n.normalize();
			n *= 0.2f;

			glColor3fv(math::vector3f::x_axis.data);
			glVertex3f(p.x.real(), p.y.real(), 0.0f);

			glColor3fv(math::vector3f::y_axis.data);
			glVertex3f(p.x.real(), p.y.real(), 0.0f);
			glVertex3f(p.x.real() + n.x, p.y.real() + n.y, 0.0f);
			glVertex3f(p.x.real(), p.y.real(), 0.0f);
		}

		for(float t = 0.0f; t <= 1.0f; t += 0.02f)
		{
			math::dual_number<float> dnt(t, 1.0f);
			const dn_vec &p = math::catmull_rom(dnt, p0, p1, p2, p3);
			math::vector2f n(p.x.dual(), p.y.dual());
			if( n.lengthSq() > 0.0f )
				n.normalize();
			n *= 0.2f;

			glColor3fv(math::vector3f::x_axis.data);
			glVertex3f(p.x.real(), p.y.real(), 0.0f);

			glColor3fv(math::vector3f::y_axis.data);
			glVertex3f(p.x.real(), p.y.real(), 0.0f);
			glVertex3f(p.x.real() + n.x, p.y.real() + n.y, 0.0f);
			glVertex3f(p.x.real(), p.y.real(), 0.0f);
		}

		for(float t = 0.0f; t <= 1.0f; t += 0.02f)
		{
			math::dual_number<float> dnt(t, 1.0f);
			const dn_vec &p = math::catmull_rom(dnt, p1, p2, p3, p3);
			math::vector2f n(p.x.dual(), p.y.dual());
			if( n.lengthSq() > 0.0f )
				n.normalize();
			n *= 0.2f;

			glColor3fv(math::vector3f::x_axis.data);
			glVertex3f(p.x.real(), p.y.real(), 0.0f);

			glColor3fv(math::vector3f::y_axis.data);
			glVertex3f(p.x.real(), p.y.real(), 0.0f);
			glVertex3f(p.x.real() + n.x, p.y.real() + n.y, 0.0f);
			glVertex3f(p.x.real(), p.y.real(), 0.0f);
		}
		glEnd();
		

		glBegin(GL_LINES);
		glColor3fv(math::vector3f::x_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::x_axis.data);

		glColor3fv(math::vector3f::y_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::y_axis.data);

		glColor3fv(math::vector3f::z_axis.data);
		glVertex3fv(math::vector3f::null_vector.data);
		glVertex3fv(math::vector3f::z_axis.data);
		glEnd();

		pDevice->swapBuffers();
	}


	return EXIT_SUCCESS;
}