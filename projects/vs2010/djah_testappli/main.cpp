#include <iostream>
#include <ctime>
#include <map>

#include <GL/glew.h>
#include <boost/thread/thread.hpp>

#include "djah/math.hpp"

#include "djah/log/console_logger.hpp"

#include "djah/utils/command.hpp"

#include "djah/video/device_base.hpp"
#include "djah/video/projection.hpp"
#include "djah/video/drivers/ogl/gl_caps.hpp"
#include "djah/video/drivers/ogl/shaders.hpp"
#include "djah/video/drivers/ogl/buffers/vertex_array.hpp"
#include "djah/video/drivers/ogl/buffers/index_buffer.hpp"
#include "djah/video/drivers/ogl/buffers/vertex_buffer.hpp"
#include "djah/video/drivers/ogl/buffers/vertex_format.hpp"

using namespace std;
using namespace djah;

#define IPHONE(S)	(S << 0)
#define IPAD(S)		(S << 1)
#define SIZE(S)		IPHONE(S)

static const char *fps_str = " FPS - ";
static std::string window_title = "Djah's Heavenly Test Application";

static const int FPS_AVG_COUNT = 10;
static long fps_avg[FPS_AVG_COUNT];
static long *cur_fps = &fps_avg[0];


utils::command *cmd = 0;

typedef std::map<std::string, utils::command*> CmdMap;
CmdMap commands;

void analyse(const std::string &cmd_str)
{
	long pos = cmd_str.find_first_of(' ');
	std::string name = cmd_str.substr(0, pos);

	CmdMap::iterator it = commands.find(name);
	if( it != commands.end() )
	{
		utils::command &cmd = *(it->second);

		std::string params = cmd_str.substr(pos+1);
		std::string ret = cmd(params);
		if(!ret.empty())
			std::cout << name << "(" << params << ") returned " << ret << std::endl;
	}
	else
	{
		log::logger::log(log::EWL_CRITICAL)	<<
			"ERROR : Command \"" << name << "\" not found"		<<
		log::logger::endl();
	}
}

void readInputs()
{
	std::string cmd_str;
	char c;
	std::cout << "> ";
	while(c = getchar())
	{
		if(c == '\n' && !cmd_str.empty())
		{
			analyse(cmd_str);
			std::cout << "> ";
			cmd_str.clear();
		}
		else if(c != '\n')
			cmd_str += c;
		else
			std::cout << "> ";
	}
}

static bool show_fps = false;
void showFPS() { show_fps = true; }
void hideFPS() { show_fps = false; }
bool getFPS()  { return show_fps; }

struct CameraTweaker
{
	CameraTweaker(video::driver_ptr driv) : driver_(driv) {}

	void projection(float fovy, float aspect, float near, float far)
	{
		driver_->setProjectionMatrix(video::make_perspective_projection(fovy, aspect, near, far));
	}

	void translate(float x, float y, float z)
	{
		math::matrix4f cam = driver_->getViewMatrix();
		cam *= math::make_translation(x, y, z);
		driver_->setViewMatrix(cam);
	}

	void rotate(float angle, float x, float y, float z)
	{
		math::matrix4f cam = driver_->getViewMatrix();
		cam *= math::make_rotation(angle, x, y, z);
		driver_->setViewMatrix(cam);
	}

	video::driver_ptr driver_;
};


static bool print_infos = false;
void printInfos() {	print_infos = true; }

using namespace djah::video::drivers;
void printInfosAux()
{
	log::logger::log(log::EWL_NOTIFICATION)
		<< "=========================================================================\n"
		<< "| Renderer                 | " << ogl::capabilities::renderer()       << "\n"
		<< "| Vendor                   | " << ogl::capabilities::vendor()         << "\n"
		<< "| OpenGL version           | " << ogl::capabilities::opengl_version() << "\n"
		<< "| GLSL version             | " << ogl::capabilities::glsl_version()   << "\n"
		<< "-------------------------------------------------------------------------\n"
		<< "| Available extensions (" << ogl::capabilities::s_extensions_.size()  << ")\n"
		<< "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
		<< log::logger::endl();

	ogl::capabilities::string_list_t::const_iterator it    = ogl::capabilities::s_extensions_.begin();
	ogl::capabilities::string_list_t::const_iterator itEnd = ogl::capabilities::s_extensions_.end();
	while(it != itEnd)
	{
		log::logger::log() << "|\t" << *it << log::logger::endl();
		++it;
	}
	log::logger::log()
		<< "========================================================================="
		<< log::logger::endl();
}

void printHelpSection(const std::string &name, const std::string &desc)
{
	log::logger::log(log::EWL_GREEN)	<< name;
	log::logger::log(log::EWL_YELLOW)	<< "\t-  ";
	log::logger::log(log::EWL_CYAN)		<< desc	<< log::logger::endl();
}

void printHelp()
{
	printHelpSection("help",								"Print this help");
	printHelpSection("infos",								"Print infos about the driver");
	printHelpSection("showFPS",								"Show the FPS counter on the title bar");
	printHelpSection("hideFPS",								"Hide the FPS counter on the title bar");
	printHelpSection("getFPS",								"Print current FPS");
	printHelpSection("projection(fovy, aspect, near, far)",	"Set the projection matrix");
	printHelpSection("translate(x,y,z)",					"Translate the current view matrix");
	printHelpSection("rotate(angle, x,y,z)",				"Rotate the current view matrix");
	printHelpSection("exit (quit)",							"Exit the application");
}


int main()
{
	std::fill(fps_avg, fps_avg+FPS_AVG_COUNT, -1);
	
	// Initialize the logger
	log::logger::setLogger(new log::console_logger);
	// Create a device
	video::device_ptr device = video::create_device( SIZE(480), SIZE(320), 16, 24, 0, false, false );
	device->setWindowTitle(window_title);
	// Retrieve the video driver
	video::driver_ptr driver = device->videoDriver();
	
	CameraTweaker cam(driver);
	CameraTweaker *pCam = &cam;

	commands["showFPS"]		= utils::make_command(&showFPS);
	commands["hideFPS"]		= utils::make_command(&hideFPS);
	commands["getFPS"]		= utils::make_command(&getFPS);
	commands["projection"]	= utils::make_command(pCam, &CameraTweaker::projection);
	commands["translate"]	= utils::make_command(pCam, &CameraTweaker::translate);
	commands["rotate"]		= utils::make_command(pCam, &CameraTweaker::rotate);
	commands["quit"]		= utils::make_command(device, &video::device_base::shutDown);
	commands["exit"]		= commands["quit"];
	commands["infos"]		= utils::make_command(&printInfos);
	commands["help"]		= utils::make_command(&printHelp);

	boost::thread testThread(&readInputs);

	GLfloat CubeArray[] = {
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	-1.0f, -1.0f, -1.0f,
		0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	-1.0f,  1.0f, -1.0f,
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	 1.0f, -1.0f,  0.0f,
		1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f,	 1.0f,  1.0f, -1.0f,
		1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 1.0f,	 1.0f, -1.0f,  1.0f,
		1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,	 1.0f,  1.0f,  1.0f,
		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f,	-1.0f, -1.0f,  1.0f,
		0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f,	-1.0f,  1.0f,  1.0f
	};

	GLuint IndiceArray[] = {
		0,1,2,1,3,2,2,3,4,3,5,4,4,5,6,5,7,6,6,7,0,7,1,0,1,7,3,7,5,3,5,6,2,6,4,2
	};

	ogl::vertex_format fvf;
	fvf.record() << ogl::format::position<float>(3) << ogl::format::color<float>(3) << ogl::format::normal<float>(3);

	/*ogl::vertex_buffer vbuf( sizeof(CubeArray), ogl::EBU_STATIC_DRAW );
	vbuf.write(CubeArray);

	ogl::index_buffer ibuf( sizeof(IndiceArray), ogl::EBU_STATIC_DRAW, ogl::EDM_TRIANGLES );
	ibuf.write(IndiceArray);

	ogl::shader sp;
	{
		ogl::vertex_shader vs("data/test.vert");
		ogl::pixel_shader  ps("data/test.frag");

		vs.compile();
		ps.compile();

		sp.attach(vs);
		sp.attach(ps);
		sp.link();
	}

	ogl::vertex_array cube(vbuf, ibuf, fvf);
	cube.init(sp);*/

	while( device->run() )
	{
		if( print_infos )
		{
			printInfosAux();
			print_infos = false;
		}

		static long elapsed = 0;
		static long fps = 0;
		long start = clock();

		// DRAW SCENE
		driver->beginScene();
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

		/*sp.begin();
		{
			// Cube transformation
			{
				static float a = 0.0f;
				math::matrix4f tr;
				tr.identity();
				tr *= math::make_translation(0.0f, 0.1f, 3.0f);
				tr *= math::make_rotation(a, math::vector3f::y_axis);
				a = a > math::pi_times_2 ? 0.0f : a+2.0f*0.005f;

				sp.sendUniformMatrix("MVP", driver->getProjectionMatrix()*driver->getViewMatrix());
				sp.sendUniformMatrix("TR", tr);
			}
			cube.draw(GL_UNSIGNED_INT);
		}
		sp.end();*/

		driver->endScene();
		device->swapBuffers();

		// FPS SHIT
		double dt = static_cast<double>((clock() - start));
		dt *= 1000.0/CLOCKS_PER_SEC; 
		elapsed += clock() - start;
		if( elapsed > CLOCKS_PER_SEC )
		{
			*cur_fps = fps;
			if( ++cur_fps >= fps_avg+FPS_AVG_COUNT )
				cur_fps = &fps_avg[0];

			if(show_fps)
			{
				fps = 0;
				int count = 0;
				for(int i = 0; i < FPS_AVG_COUNT; ++i)
					if(fps_avg[i] != -1)
					{
						fps += fps_avg[i];
						++count;
					}
				fps /= count;

				std::stringstream ss;
				ss << fps << fps_str << window_title << " (" << /*(clock()/CLOCKS_PER_SEC)*/ dt << " ms)";
				device->setWindowTitle(ss.str());
			}

			fps = 0;
			elapsed = 0;
		}
		else
			++fps;
	}

	device->shutDown();
	delete device;

	return 0;
}