#include <iostream>
#include <thread>

#include <djah/core/randomizer.hpp>
#include <djah/core/events.hpp>
#include <djah/core/state_machine.hpp>

#include <djah/debug/assertion.hpp>
#include <djah/debug/xml_sink.hpp>
#include <djah/debug/console_sink.hpp>
#include <djah/debug/core_logger.hpp>

#include <djah/system/device.hpp>
#include <djah/system/driver.hpp>

#include <djah/system/input/mouse.hpp>
#include <djah/system/input/keyboard.hpp>
#include <djah/system/input/gamepad.hpp>

#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>

#include <djah/resources/asset_finder.hpp>
#include <djah/resources/config_object.hpp>

#include <djah/gameplay/game_object.hpp>
#include <djah/gameplay/component_serializer.hpp>

#include <djah/game/components_registry.hpp>
#include <djah/game/processes/actions_process.hpp>

#include <djah/opengl.hpp>

#include <djah/3d/shader.hpp>

#include <djah/math.hpp>

#include "resource_finder.hpp"

using namespace djah;
namespace gpc = djah::game::components;

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
	debug::sink_sptr pOpenGLSink    ( new debug::xml_sink("log/opengl.log.xml"    , debug::log_filter("opengl")) );
	debug::sink_sptr pSystemSink    ( new debug::xml_sink("log/system.log.xml"    , debug::log_filter("system")) );
	debug::sink_sptr pFileSystemSink( new debug::xml_sink("log/filesystem.log.xml", debug::log_filter("fs")) );
	debug::sink_sptr pGlobalSink    ( new debug::xml_sink("log/global.log.xml"    , debug::log_filter("any")) );

	debug::sink_sptr pConsoleSink   ( new debug::console_sink     ( debug::log_filter("any") ) );
	debug::sink_sptr pDebugSink     ( new debug::output_debug_sink( debug::log_filter("any") ) );


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


struct renderer_config
{
	renderer_config()
		: maxAnisotropy(1.0f)
		, clearColor(0.0f,0.0f,0.0f)
		, bilinearMode(opengl::sampler::eBM_None)
		, mipmappingMode(opengl::sampler::eMM_None)
	{}

	float			maxAnisotropy;
	math::vector3f	clearColor;
	int				bilinearMode;
	int				mipmappingMode;
};

class renderer_cfg
	: public resources::config_object<renderer_config>
{
public:
	renderer_cfg(const std::string &url)
		: resources::config_object<renderer_config>(url)
	{
		list_config_vars
		(
			maxAnisotropy,
			clearColor,
			bilinearMode,
			mipmappingMode
		);
	}
};

/*
#define BEGIN_CONFIG(ConfigName)\
struct ConfigName ## _cfg : public resources::config_object<ConfigName ## _config>\
{\
	ConfigName ## _cfg(const std::string &url)\
		: resources::config_object<ConfigName ## _config>(url)\
	{\
		list_config_vars(

#define END_CONFIG() ); } }

BEGIN_CONFIG(renderer)
	maxAnisotropy
END_CONFIG()
*/

typedef gameplay::game_object<game::components::DefaultComponentTypes> game_object_t;

namespace djah { namespace opengl { template<typename T> class uniform {}; } }


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



class floret
{
public:

	void build(float smallRadius)
	{
		const int nbPetals = 6;
		const int nbTrianglesPerPetal = 16;
		const int nbTriangles = nbPetals * nbTrianglesPerPetal;

		const int nbIndices = nbTriangles * 3;
		u16 indices[nbIndices];
		for(int i = 0, j = 0; i < nbIndices-1; i+=nbTrianglesPerPetal*3, j+=6)
		{
			int k = i-1;
			// bottom
			indices[++k] = 0;
			indices[++k] = j+2;
			indices[++k] = j+4;
			indices[++k] = 0;
			indices[++k] = j+4;
			indices[++k] = j+6;
			indices[++k] = 0;
			indices[++k] = j+6;
			indices[++k] = j+8;
			// top		 
			indices[++k] = 1;
			indices[++k] = j+3;
			indices[++k] = j+5;
			indices[++k] = 1;
			indices[++k] = j+5;
			indices[++k] = j+7;
			indices[++k] = 1;
			indices[++k] = j+7;
			indices[++k] = j+9;
			// 1st wall	 
			indices[++k] = 0;
			indices[++k] = j+2;
			indices[++k] = 1;
			indices[++k] = 1;
			indices[++k] = j+2;
			indices[++k] = j+3;
			// 2nd wall
			indices[++k] = j+2;
			indices[++k] = j+4;
			indices[++k] = j+3;
			indices[++k] = j+3;
			indices[++k] = j+4;
			indices[++k] = j+5;
			// 3rd wall
			indices[++k] = j+4;
			indices[++k] = j+6;
			indices[++k] = j+5;
			indices[++k] = j+5;
			indices[++k] = j+6;
			indices[++k] = j+7;
			// 4th wall
			indices[++k] = j+6;
			indices[++k] = j+8;
			indices[++k] = j+7;
			indices[++k] = j+7;
			indices[++k] = j+8;
			indices[++k] = j+9;
			// 5th wall
			indices[++k] = j+8;
			indices[++k] = 0;
			indices[++k] = j+9;
			indices[++k] = j+9;
			indices[++k] = 0;
			indices[++k] = 1;
		}
		//indices[nbIndices-4] = 2;
		//indices[nbIndices-1] = 3;

		const int nbVertices = (1 + nbPetals * 3) * 2 + 2;
		math::vector3f vertices[nbVertices];
		math::vector3f colors[nbVertices];
		vertices[0] = math::vector3f(0.0f, 0.0f, 0.0f);
		vertices[1] = math::vector3f(0.0f, 0.0f, 10.0f);
		colors[0] = math::vector3f(1.0f, 1.0f, 1.0f);
		colors[1] = math::vector3f(1.0f, 1.0f, 1.0f);

		const float smallAngularStep = 0.3334731722518321153360907553516010700659003898479909f;
		const float bigAngularStep   = 0.3802512066929335154820329503899654879339223534290533f;
		float angleAccum = -0.190125603f;//
		int useSmallRadius = 0;
		int useSmallAngularStep = 1;
		const float bigRadius = 1.32287565553f; //sqrt(7)/2
		for(int i = 2; i < nbVertices; i+=2)
		{
			const float radius = useSmallRadius == 0 ? smallRadius : bigRadius;
			useSmallRadius = (useSmallRadius+1) % 3;
			vertices[i]   = math::vector3f(radius * cos(angleAccum), radius * sin(angleAccum), 0.0f);
			vertices[i+1] = math::vector3f(vertices[i].x, vertices[i].y, 1.0f);
			colors[i]   = vertices[i];
			colors[i+1] = vertices[i+1];
			const float angularStep = useSmallAngularStep == 2 ? bigAngularStep : smallAngularStep;
			useSmallAngularStep = (useSmallAngularStep+1)%3;
			angleAccum += angularStep;
		}

		pVB = new opengl::vertex_buffer(sizeof(vertices)+sizeof(colors), opengl::eBU_StaticDraw);
		//pwVB = new opengl::vertex_buffer(sizeof(vertices), opengl::eBU_StaticDraw);
		pIB = new opengl::index_buffer(sizeof(indices), opengl::eBU_StaticDraw);
		//pwIB = new opengl::index_buffer(sizeof(windices), opengl::eBU_StaticDraw, opengl::EDM_LINE_STRIP);

		auto offset = pVB->write(vertices);
		pVB->write(colors, offset);
		//pwVB->write(vertices);
		pIB->write(indices);
		//pwIB->write(windices);

		opengl::vertex_format vertexFormat(opengl::vertex_format::ePT_Packed);
		vertexFormat << opengl::format::position<3,float>() << opengl::format::color<3,float>();

		pVA = new opengl::vertex_array;
		pVA->setIndexBuffer(pIB);
		pVA->addVertexBuffer(pVB, vertexFormat);
		pVA->setVertexCount(nbVertices);

		//pwVA = new opengl::vertex_array;
		//pwVA->setIndexBuffer(pwIB);
		//pwVA->addVertexBuffer(pwVB, vertexFormat);
		//pwVA->setVertexCount(nbVertices);
	}

	void build()
	{
		const int nbPetals = 6;
		const int nbTrianglesPerPetal = 16;
		const int nbTriangles = nbPetals * nbTrianglesPerPetal;
		const int nbVerticesPerPetal = nbTrianglesPerPetal * 3;

		const int nbVertices = nbTriangles * 3 * 2;
		math::vector3f vertices[nbVertices];

		const int nbLinesPerPetal = 15;
		const int nbLines = nbLinesPerPetal * nbPetals;
		math::vector3f wireframe[nbLines*2];

		const float smallAngularStep = 0.3334731722518321153360907553516010700659003898479909f;
		const float bigAngularStep   = 0.3802512066929335154820329503899654879339223534290533f;
		const float angleStep = smallAngularStep * 2.0f + bigAngularStep;
		float angleAccum = -0.190125603f;
		for(int i = 0, w = 0; i < nbVertices; i+=nbVerticesPerPetal*2, w+=nbLinesPerPetal*2)
		{
			buildPetal(vertices+i, wireframe+w, angleAccum);
			angleAccum += angleStep;
		}

		pVB = new opengl::vertex_buffer(sizeof(vertices), opengl::eBU_StaticDraw);
		pVB->write(vertices);

		opengl::vertex_format vertexFormat;
		vertexFormat
			<< opengl::format::position<3,float>()
			<< opengl::format::normal<3,float>();

		pVA = new opengl::vertex_array;
		pVA->addVertexBuffer(pVB, vertexFormat);
		pVA->setVertexCount(nbTriangles * 3);

		pwVB = new opengl::vertex_buffer(sizeof(wireframe), opengl::eBU_StaticDraw);
		pwVB->write(wireframe);

		opengl::vertex_format vertexFormatW;
		vertexFormatW << opengl::format::position<3,float>();

		pwVA = new opengl::vertex_array;
		pwVA->addVertexBuffer(pwVB, vertexFormatW);
		pwVA->setVertexCount(nbLines * 2);
	}

	void buildPetal(math::vector3f *vertices, math::vector3f *wireframe, float angle)
	{
		const float smallAngularStep = 0.3334731722518321153360907553516010700659003898479909f;
		const float bigAngularStep   = 0.3802512066929335154820329503899654879339223534290533f;

		const float smallRadius = 1.0f;
		const float bigRadius = 1.32287565553f; //sqrt(7)/2

		const float h = 0.25f;//utils::randomizer::random(-0.2f, 0.2f);

		const math::vector3f p1b(smallRadius*cos(angle),										smallRadius*sin(angle),											0.0f*h);
		const math::vector3f p2b(bigRadius*cos(angle+smallAngularStep),							bigRadius*sin(angle+smallAngularStep),							0.0f*h);
		const math::vector3f p3b(bigRadius*cos(angle+smallAngularStep+bigAngularStep),			bigRadius*sin(angle+smallAngularStep+bigAngularStep),			0.0f*h);
		const math::vector3f p4b(smallRadius*cos(angle+2.0f*smallAngularStep+bigAngularStep),	smallRadius*sin(angle+2.0f*smallAngularStep+bigAngularStep),	0.0f*h);

		const math::vector3f p1t(p1b.x, p1b.y, 1.0f*h);
		const math::vector3f p2t(p2b.x, p2b.y, 1.0f*h);
		const math::vector3f p3t(p3b.x, p3b.y, 1.0f*h);
		const math::vector3f p4t(p4b.x, p4b.y, 1.0f*h);

		const math::vector3f b(0.0f, 0.0f, 0.0f*h);
		const math::vector3f t(0.0f, 0.0f, 1.0f*h);
		const math::vector3f up(0.0f, 0.0f, 1.0f);
		const math::vector3f down(0.0f, 0.0f, -1.0f);

		int i = -1;
		int w = -1;
		math::vector3f *normals = vertices;
		// bottom
		vertices[++i] = b;		normals[++i] = down;	vertices[++i] = p1b;	normals[++i] = down;	vertices[++i] = p2b;	normals[++i] = down;
		vertices[++i] = b;		normals[++i] = down;	vertices[++i] = p2b;	normals[++i] = down;	vertices[++i] = p3b;	normals[++i] = down;
		vertices[++i] = b;		normals[++i] = down;	vertices[++i] = p3b;	normals[++i] = down;	vertices[++i] = p4b;	normals[++i] = down;
		// top
		vertices[++i] = t;		normals[++i] = up;		vertices[++i] = p1t;	normals[++i] = up;		vertices[++i] = p2t;	 normals[++i] = up;
		vertices[++i] = t;		normals[++i] = up;		vertices[++i] = p2t;	normals[++i] = up;		vertices[++i] = p3t;	 normals[++i] = up;
		vertices[++i] = t;		normals[++i] = up;		vertices[++i] = p3t;	normals[++i] = up;		vertices[++i] = p4t;	 normals[++i] = up;
		// 1st wall
		vertices[++i] = t;		normals[++i] = up;		vertices[++i] = b;		normals[++i] = up;		vertices[++i] = p1b;	normals[++i] = up;
		vertices[++i] = t;		normals[++i] = up;		vertices[++i] = p1b;	normals[++i] = up;		vertices[++i] = p1t;	normals[++i] = up;
		// 2nd wall
		vertices[++i] = p1t;	normals[++i] = up;		vertices[++i] = p1b;	normals[++i] = up;		vertices[++i] = p2b;	normals[++i] = up;
		vertices[++i] = p1t;	normals[++i] = up;		vertices[++i] = p2b;	normals[++i] = up;		vertices[++i] = p2t;	normals[++i] = up;
		// 3rd wall
		vertices[++i] = p2t;	normals[++i] = up;		vertices[++i] = p2b;	normals[++i] = up;		vertices[++i] = p3b;	normals[++i] = up;
		vertices[++i] = p2t;	normals[++i] = up;		vertices[++i] = p3b;	normals[++i] = up;		vertices[++i] = p3t;	normals[++i] = up;
		// 4th wall
		vertices[++i] = p3t;	normals[++i] = up;		vertices[++i] = p3b;	normals[++i] = up;		vertices[++i] = p4b;	normals[++i] = up;
		vertices[++i] = p3t;	normals[++i] = up;		vertices[++i] = p4b;	normals[++i] = up;		vertices[++i] = p4t;	normals[++i] = up;
		// 5th wall
		vertices[++i] = p4t;	normals[++i] = up;		vertices[++i] = p4b;	normals[++i] = up;		vertices[++i] = b;		normals[++i] = up;
		vertices[++i] = p4t;	normals[++i] = up;		vertices[++i] = b;		normals[++i] = up;		vertices[++i] = t;		normals[++i] = up;

		//bottom
		wireframe[++w] = b;			wireframe[++w] = p1b;
		wireframe[++w] = p1b;		wireframe[++w] = p2b;
		wireframe[++w] = p2b;		wireframe[++w] = p3b;
		wireframe[++w] = p3b;		wireframe[++w] = p4b;
		wireframe[++w] = p4b;		wireframe[++w] = b;
		//top
		wireframe[++w] = t;			wireframe[++w] = p1t;
		wireframe[++w] = p1t;		wireframe[++w] = p2t;
		wireframe[++w] = p2t;		wireframe[++w] = p3t;
		wireframe[++w] = p3t;		wireframe[++w] = p4t;
		wireframe[++w] = p4t;		wireframe[++w] = t;
		//walls
		wireframe[++w] = t;			wireframe[++w] = b;
		wireframe[++w] = p1t;		wireframe[++w] = p1b;
		wireframe[++w] = p2t;		wireframe[++w] = p2b;
		wireframe[++w] = p3t;		wireframe[++w] = p3b;
		wireframe[++w] = p4t;		wireframe[++w] = p4b;
	}

	opengl::vertex_array  *pVA;
	opengl::vertex_buffer *pVB;
	opengl::index_buffer  *pIB;

	opengl::vertex_array  *pwVA;
	opengl::vertex_buffer *pwVB;
	opengl::index_buffer  *pwIB;
};


#include "djah/core/command.hpp"
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

		const std::string &params = cmd_str.substr(pos+1);
		const std::string &ret = cmd(params);
		if( !ret.empty() )
		{
			std::cout << name << "(" << params << ") returned " << ret << std::endl;
		}
	}
	else
	{
		DJAH_GLOBAL_ERROR()	<< "Command \"" << name << "\" not found" << DJAH_END_LOG();
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
			std::flush(std::cout);
			cmd_str.clear();
		}
		else if(c != '\n')
		{
			cmd_str += c;
		}
		else
		{
			std::cout << "> ";
		}
	}
}

#include "djah/opengl/buffers/enum_buffer_target.hpp"

int main()
{
	std::cout << opengl::BufferTarget::Descriptors[opengl::BufferTarget::AtomicCounter].glEnumStr << std::endl;
	initLoggers();

	// 0 - Init system
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./config"));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./save/assets", false, 0));
	filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./assets", false, 1));
	filesystem::browser::get().addSavingChannel(new filesystem::directory_source("./save/assets"));

	resources::asset_finder<> assetFinder;
	assetFinder.registerExtensions<resources::mesh>("mesh");
	assetFinder.registerExtensions<resources::data_object<>>("config json");


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
	glDebugMessageCallback((GLDEBUGPROC)oglDebugProc, nullptr);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	auto testJsonConfig = assetFinder.get<resources::data_object<>>("configTest.json");

	auto pRendererConfig = resources::open_config<renderer_cfg>("renderer.config");
	const math::vector3f &cc = pRendererConfig->clearColor;
	glClearColor(cc[0], cc[1], cc[2], 1.0f);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(1.5f);
	system::input::mouse mouse;
	system::input::keyboard kb;
	system::input::gamepad gp(0);
	bool fs = pDeviceConfig->fullscreen;
	bool resolutionChanged = true;

	// 3.5 - Load resources
	floret fl;
	fl.build();
	d3d::shader simpleShader("lit_textured");
	d3d::shader *uniformColorShader = nullptr;
	auto pMesh    = assetFinder.get<resources::mesh>("meshes/feisar.mesh");
	//auto pTexture = assetFinder.get<resources::texture>("feisar-diffuse.png");
	auto pTexture = d3d::texture_manager::get().find("feisar-diffuse.png");
	auto pSpecular = d3d::texture_manager::get().find("feisar-specular.jpg");


	game::processes::actions_process<gpc::DefaultComponentTypes> actionsProcess(mouse, kb, gp);

	// 3.6 - Camera
	game_object_t camera("cameras/main.camera");
	gameplay::game_object_serializer<gpc::DefaultComponentTypes>::deserialize(camera);
	actionsProcess.add(&camera);

	gameplay::component<gpc::transform> cameraTransform = camera.get<gpc::transform>();
	gameplay::component<gpc::fov>		cameraFOV		= camera.get<gpc::fov>();
	gameplay::component<gpc::action_map> cameraAM		= camera.get<gpc::action_map>();

	const math::vector3f neutralDirection(0.0f,0.0f,-1.0f);
	const math::vector3f upDirection(0.0f,1.0f,0.0f);
	const math::vector3f &realDirection   = math::rotate(cameraTransform->orientation, neutralDirection);
	const math::vector3f &cameraCenter    = cameraTransform->position + realDirection;
	const math::vector3f &realUpDirection = math::rotate(cameraTransform->orientation, upDirection);
	
	math::matrix4f w;
	math::matrix4f v = math::make_look_at(cameraTransform->position, cameraCenter, realUpDirection);
	math::matrix4f p;
	math::matrix4f vp;
	math::matrix4f wvp;
	
	//opengl::uniform<math::matrix4f> in_World = simpleShader.getUniform<math::matrix4f>("in_World");
	//in_World = math::matrix4f::identity;
	//in_World.send();

	// Low
	std::shared_ptr<opengl::sampler> pLowQualitySampler = std::make_shared<opengl::sampler>();
	pLowQualitySampler->setFiltering(opengl::sampler::eBM_None, opengl::sampler::eMM_None, 1.0f);
	// High
	std::shared_ptr<opengl::sampler> pHighQualitySampler = std::make_shared<opengl::sampler>();
	pHighQualitySampler->setFiltering(opengl::sampler::eBM_NearFar, opengl::sampler::eMM_Trilinear, opengl::capabilities::value_of<float>(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT));
	// Custom
	std::shared_ptr<opengl::sampler> pCustomSampler = std::make_shared<opengl::sampler>();
	pCustomSampler->setFiltering(opengl::sampler::eBilinearMode(pRendererConfig->bilinearMode), opengl::sampler::eMipmappingMode(pRendererConfig->mipmappingMode), pRendererConfig->maxAnisotropy);



	enum eTextureUnit
	{
		eTU_DiffuseMap	= 0,
		eTU_NormalMap	= 1,
		eTU_SpecularMap	= 2,

		eTU_Count
	};

	opengl::texture_unit<eTU_DiffuseMap>::activate_and_bind(pTexture, pHighQualitySampler);
	opengl::texture_unit<eTU_SpecularMap>::activate_and_bind(pSpecular, pHighQualitySampler);

	simpleShader.program().begin();
	simpleShader.program().sendUniform("in_World", math::matrix4f::identity);
	simpleShader.program().sendUniform("in_DiffuseSampler", int(eTU_DiffuseMap));
	simpleShader.program().sendUniform("in_SpecularSampler", int(eTU_SpecularMap));
	simpleShader.program().end();
	pMesh->init(simpleShader.program());

	float currentAngle = 0.0f;
	float targetAngle = 0.0f;
	float maxAngle = 30.0f;


	commands["quit"]		= utils::make_command(pDevice, &system::device::shutDown);
	commands["exit"]		= commands["quit"];
	commands["set_resolution"] = utils::make_command<bool, system::device, system::device_sptr, int, int, bool>(pDevice, &system::device::setResolution);

	std::thread t1(readInputs);

	// 4 - Run app
	while( pDevice->run() )
	{
		if( pDevice->hasFocus() )
		{
			mouse.update();
			kb.update();
			gp.update();
		}

		actionsProcess.execute(0.0f);

		if( kb.pressed(system::input::eKC_ESCAPE) )
		{
			pDevice->shutDown();
		}

		if( kb.pressed(system::input::eKC_F) )
		{
			fs = !fs;
			pDevice->setResolution(fs);
			resolutionChanged = true;
		}

		if( kb.pressed(system::input::eKC_F5) )
		{
			gameplay::game_object_serializer<gpc::DefaultComponentTypes>::deserialize(camera);

			const math::vector3f &realDirection   = math::rotate(cameraTransform->orientation, neutralDirection);
			const math::vector3f &realUpDirection = math::rotate(cameraTransform->orientation, upDirection);
			const math::vector3f &cameraCenter    = cameraTransform->position + realDirection;
			const float aspectRatio = float(std::max(pDeviceConfig->width, pDeviceConfig->height))
									/ float(std::min(pDeviceConfig->width, pDeviceConfig->height));
			v = math::make_look_at(cameraTransform->position, cameraCenter, realUpDirection);
			p = math::make_perspective_projection
			(
				math::radian<float>::from_degree(cameraFOV->horizontal),
				cameraFOV->distortion * aspectRatio,
				cameraFOV->nearPlane, cameraFOV->farPlane
			);
			vp = v*p;

			pRendererConfig = resources::open_config<renderer_cfg>("renderer.config");
			const math::vector3f &cc = pRendererConfig->clearColor;
			glClearColor(cc[0], cc[1], cc[2], 1.0f);
		}

		if( !uniformColorShader || kb.pressed(system::input::eKC_F5) )
		{
			if( uniformColorShader )
			{
				delete uniformColorShader;
			}
			uniformColorShader = new d3d::shader("uniform_color");
			fl.pVA->init(uniformColorShader->program());
			fl.pwVA->init(uniformColorShader->program());
			uniformColorShader->program().begin();
			uniformColorShader->program().sendUniform("in_Color", math::vector3f(1.0f,1.0f,1.0f));
			uniformColorShader->program().end();
		}

		if( kb.pressed(system::input::eKC_P) )
		{
			static bool wireframe = false;
			wireframe = !wireframe;
			glPolygonMode(GL_FRONT_AND_BACK , wireframe ? GL_LINE : GL_FILL);
		}

		if( kb.pressed(system::input::eKC_L) )
		{
			opengl::texture_unit<eTU_DiffuseMap>::activate_and_bind(pTexture, pLowQualitySampler);
			opengl::texture_unit<eTU_SpecularMap>::activate_and_bind(pSpecular, pLowQualitySampler);
		}

		if( kb.pressed(system::input::eKC_H) )
		{
			opengl::texture_unit<eTU_DiffuseMap>::activate_and_bind(pTexture, pHighQualitySampler);
			opengl::texture_unit<eTU_SpecularMap>::activate_and_bind(pSpecular, pHighQualitySampler);
		}

		if( kb.pressed(system::input::eKC_C) )
		{
			opengl::texture_unit<eTU_DiffuseMap>::activate_and_bind(pTexture, pCustomSampler);
			opengl::texture_unit<eTU_SpecularMap>::activate_and_bind(pSpecular, pCustomSampler);
		}

		if( resolutionChanged )
		{
			const float aspectRatio = float(std::max(pDeviceConfig->width, pDeviceConfig->height))
									/ float(std::min(pDeviceConfig->width, pDeviceConfig->height));

			p = math::make_perspective_projection
			(
				math::radian<float>::from_degree(cameraFOV->horizontal),
				cameraFOV->distortion * aspectRatio,
				cameraFOV->nearPlane, cameraFOV->farPlane
			);

			//p = math::make_orthographic_projection(0.0f,float(pDeviceConfig->width),float(pDeviceConfig->width), 0.0f, -1.0f,100.0f);
			vp = v*p;
			glViewport(0,0,pDeviceConfig->width, pDeviceConfig->height);
			resolutionChanged = false;
		}

		if( gp.isPlugged() )
		{
			const float leftEngine = gp.getTrigger(system::input::eX360_LeftTrigger).value();
			const float rightEngine = gp.getTrigger(system::input::eX360_RightTrigger).value();
			targetAngle = (rightEngine - leftEngine) * maxAngle;

			//gp.vibrate(leftEngine, rightEngine);

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

		// Move camera
		{
			const float camSpeed = 1.0f;
			const math::vector3f &disp = camSpeed * math::vector3f
			(
				cameraAM->states["MOVE_RIGHT"]	  - cameraAM->states["MOVE_LEFT"],
				cameraAM->states["MOVE_UPWARD"]	  - cameraAM->states["MOVE_DOWNWARD"],
				cameraAM->states["MOVE_BACKWARD"] - cameraAM->states["MOVE_FORWARD"]
			);

			bool turned = false;
			const float turnSpeed = 2.7f;

			const float horizontalTurn = cameraAM->states["LOOK_RIGHT"] - cameraAM->states["LOOK_LEFT"];
			if( std::abs(horizontalTurn) > 0.0f )
			{
				const math::vector3f &realUpDirection = math::rotate(cameraTransform->orientation, upDirection);
				const math::quatf &nrot = math::make_quaternion(math::radian<float>::from_degree(-horizontalTurn*turnSpeed), upDirection);
				cameraTransform->orientation = nrot * cameraTransform->orientation;
				turned = true;
			}
			
			const float verticalTurn = cameraAM->states["LOOK_UP"] - cameraAM->states["LOOK_DOWN"];
			if( std::abs(verticalTurn) > 0.0f )
			{
				const math::vector3f realRightDirection(-1.0f, 0.0f, 0.0f);
				const math::quatf &nrot = math::make_quaternion(math::radian<float>::from_degree(verticalTurn*turnSpeed), realRightDirection);
				cameraTransform->orientation = cameraTransform->orientation * nrot;
				turned = true;
			}

			if( turned || disp != math::vector3f::null_vector )
			{
				const math::vector3f &realDisp		  = math::rotate(cameraTransform->orientation, disp);
				cameraTransform->position += realDisp;
				const math::vector3f &realDirection   = math::rotate(cameraTransform->orientation, neutralDirection);
				const math::vector3f &realUpDirection = math::rotate(cameraTransform->orientation, upDirection);
				const math::vector3f &cameraCenter    = cameraTransform->position + realDirection;
				v = math::make_look_at(cameraTransform->position, cameraCenter, upDirection);
				vp = v*p;
			}
		}

		w = math::make_rotation(math::radian<float>::from_degree(currentAngle), math::vector3f::z_axis);
		wvp = w*vp;

		pDriver->beginFrame();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/**/
		simpleShader.program().begin();
		simpleShader.program().sendUniform("in_World", w);
		simpleShader.program().sendUniform("in_EyeWorldPos", math::resize<3>(math::apply_transform(w, math::point3_to_point4(cameraTransform->position))));
		simpleShader.program().sendUniform("in_WVP", wvp);
		pMesh->draw();
		simpleShader.program().end();
		/**
		uniformColorShader->program().begin();
		uniformColorShader->program().sendUniform("in_WVP", wvp);
		static const int nbRings = 50;
		static const int nbFlorets = 3 * nbRings * (nbRings - 1) + 1;
		uniformColorShader->program().sendUniform("in_Color", math::vector3f(1,1,1));
		uniformColorShader->program().sendUniform("in_Wireframe", 0);
		fl.pVA->drawInstanced(nbFlorets);
		uniformColorShader->program().sendUniform("in_Color", math::vector3f(1,0,0));
		uniformColorShader->program().sendUniform("in_Wireframe", 1);
		fl.pwVA->drawInstanced(nbFlorets, GL_LINES);
		uniformColorShader->program().end();
		/**/
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(&wvp[0][0]);
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

// 		glEnable(GL_TEXTURE_2D);
// 		glColor3f(1.0f, 1.0f, 1.0f);
// 		//opengl::texture_unit<eTU_DiffuseMap>::activate_and_bind(pTexture, pHighQualitySampler);
// 		glBegin(GL_QUADS);
// 		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f,  0.0f,  0.0f);
// 		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f,  10.0f,  0.0f);
// 		glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 10.0f, 0.0f);
// 		glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 0.0f, 0.0f);
// 		glEnd();
		
		/**/
		pDriver->endFrame();
	}

	gameplay::game_object_serializer<gpc::DefaultComponentTypes>::serialize(camera);

	t1.detach();
	opengl::texture_unit<eTU_DiffuseMap>::set_active();
	opengl::texture_unit<eTU_DiffuseMap>::unbind();
	opengl::texture_unit<eTU_SpecularMap>::set_active();
	opengl::texture_unit<eTU_SpecularMap>::unbind();

	return EXIT_SUCCESS;
}