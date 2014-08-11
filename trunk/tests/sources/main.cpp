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

#include <djah/gameplay/entity.hpp>
#include <djah/gameplay/component_serializer.hpp>

#include <djah/game/components_registry.hpp>
#include <djah/game/processes/actions_process.hpp>
#include <djah/game/processes/rendering_process.hpp>
#include <djah/game/components/enum_test.hpp>

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


typedef gameplay::entity_t<game::components::DefaultComponentTypes> Entity;


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

class World
{
public:
    World(const std::string &fileName)
        : fileName_(fileName)
    {}

    template<typename Process>
    void load(Process &renderingProcess)
    {
        filesystem::stream_sptr pStream = filesystem::browser::get().openReadStream(fileName_);

        if( pStream == nullptr )
        {
            return;
        }
        
        if( pStream->size() == 0 )
        {
            return;
        }

        filesystem::memory_stream memStrm(pStream.get());
        const std::string &jsonStr = memStrm.toString();

        rapidjson::Document doc;
        doc.Parse<0>(jsonStr.c_str());

        if( doc.HasParseError() )
        {
            return;
        }

        std::set<std::string> worldEntities;

        auto itEnd = doc.MemberEnd();
        for(auto it = doc.MemberBegin(); it != itEnd; ++it)
        {
            if( ensure(it->value.IsObject()) )
            {
                const std::string &entityName = it->name.GetString();
                
                auto itEntity = entities_.find(entityName);
                if( itEntity == entities_.end() )
                {
                    itEntity = entities_.insert( std::map<std::string, Entity>::value_type(entityName, Entity(entityName)) ).first;
                }
                else
                {
                    renderingProcess.remove(&(itEntity->second));
                }

                check(itEntity != entities_.end());

                gameplay::component_serializer<gpc::DefaultComponentTypes>::deserialize(itEntity->second, it->value);

                renderingProcess.add(&(itEntity->second));
                worldEntities.insert(entityName);
            }
        }

        // Remove orphaned entities
        const auto &entitiesItEnd = worldEntities.end();
        auto it = entities_.begin();
        while( it != entities_.end() )
        {
            auto itEntity = worldEntities.find(it->first);
            if( itEntity == entitiesItEnd )
            {
                renderingProcess.remove(&(it->second));
                it = entities_.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

private:
    std::string fileName_;
    std::map<std::string, Entity> entities_;
};

struct directory_changed_handler
{
    LPCWSTR lpDir_;
    std::function<void()> callback_;
};

bool needToReloadWorld = false;
bool needToRefreshMeshes = false;
bool needToRefreshTextures = false;

void WatchDirectory(std::vector<directory_changed_handler> &handlers)
{
    DWORD dwWaitStatus; 

    std::vector<HANDLE> dwChangeHandles(handlers.size());
    int i = 0;
    std::for_each(handlers.begin(), handlers.end(), [&dwChangeHandles, &i](directory_changed_handler &handler)
    {
        dwChangeHandles[i] = FindFirstChangeNotification(handler.lpDir_, FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);

        if (dwChangeHandles[i] == INVALID_HANDLE_VALUE) 
        {
            printf("\n ERROR: FindFirstChangeNotification function failed.\n");
            ExitProcess(GetLastError()); 
        }

        if (dwChangeHandles[i] == NULL)
        {
            printf("\n ERROR: Unexpected NULL from FindFirstChangeNotification.\n");
            ExitProcess(GetLastError()); 
        }

        ++i;
    });

    while( true ) 
    { 
        // Wait for notification.
        dwWaitStatus = WaitForMultipleObjects(dwChangeHandles.size(), &dwChangeHandles[0], FALSE, INFINITE); 

        if( dwWaitStatus >= WAIT_OBJECT_0 && dwWaitStatus < WAIT_OBJECT_0 + dwChangeHandles.size() )
        { 
            handlers[dwWaitStatus - WAIT_OBJECT_0].callback_();
            if ( FindNextChangeNotification(dwChangeHandles[dwWaitStatus - WAIT_OBJECT_0]) == FALSE )
            {
                printf("\n ERROR: FindNextChangeNotification function failed.\n");
                ExitProcess(GetLastError()); 
            }
        }
    }
}


int main()
{
    initLoggers();

    // 0 - Init system
    filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./config"));
    filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./save/assets", false, 0));
    filesystem::browser::get().addLoadingChannel(new filesystem::directory_source("./assets", false, 1));
    filesystem::browser::get().addSavingChannel(new filesystem::directory_source("./save/assets"));

    resources::default_asset_finder::get().registerExtensions<resources::mesh>("mesh");
    resources::default_asset_finder::get().registerExtensions<resources::data_object<>>("config json");
    resources::default_asset_finder::get().registerExtensions<resources::image>("png jpg tga");


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

    auto testJsonConfig = resources::default_asset_finder::get().load<resources::data_object<>>("configTest.json");

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

    game::processes::rendering_process<gpc::DefaultComponentTypes> renderingProcess;
    World world("worlds/test.world");
    world.load(renderingProcess);

    auto pNormal   = d3d::texture_manager::get().find("houseN.jpg");
    auto pSpecular = d3d::texture_manager::get().find("feisar-specular.jpg");



    // 3.6 - Camera
    Entity camera("cameras/main.camera");
    gameplay::component_serializer<gpc::DefaultComponentTypes>::deserialize(camera);

    Entity entityTest("archetypes/test.archetype");
    gameplay::component_serializer<gpc::DefaultComponentTypes>::deserialize(entityTest);

    game::processes::actions_process<gpc::DefaultComponentTypes> actionsProcess(mouse, kb, gp);
    actionsProcess.add(&camera);

    gameplay::component<gpc::transform> cameraTransform = camera.get<gpc::transform>();
    gameplay::component<gpc::fov>		cameraFOV		= camera.get<gpc::fov>();
    gameplay::component<gpc::action_map> cameraAM		= camera.get<gpc::action_map>();

    const math::vector3f neutralDirection(0.0f,-1.0f,0.0f);
    const math::vector3f upDirection(0.0f,0.0f,1.0f);
    const math::vector3f &realDirection   = math::rotate(cameraTransform->orientation, neutralDirection);
    const math::vector3f &cameraCenter    = cameraTransform->position + realDirection;
    const math::vector3f &realUpDirection = math::rotate(cameraTransform->orientation, upDirection);
    
    math::matrix4f w;
    math::matrix4f v = math::make_look_at(cameraTransform->position, cameraCenter, realUpDirection);
    math::matrix4f p;
    math::matrix4f vp;
    math::matrix4f wvp;

    // Low
    std::shared_ptr<opengl::sampler> pLowQualitySampler = std::make_shared<opengl::sampler>();
    pLowQualitySampler->setFiltering(opengl::sampler::eBM_None, opengl::sampler::eMM_None, 1.0f);
    // High
    std::shared_ptr<opengl::sampler> pHighQualitySampler = std::make_shared<opengl::sampler>();
    pHighQualitySampler->setFiltering(opengl::sampler::eBM_NearFar, opengl::sampler::eMM_Trilinear, opengl::capabilities::value_of<float>(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT));
    // Custom
    std::shared_ptr<opengl::sampler> pCustomSampler = std::make_shared<opengl::sampler>();
    pCustomSampler->setFiltering(opengl::sampler::eBilinearMode(pRendererConfig->bilinearMode), opengl::sampler::eMipmappingMode(pRendererConfig->mipmappingMode), pRendererConfig->maxAnisotropy);


    renderingProcess.setSamplers(pLowQualitySampler, pHighQualitySampler);

    enum eTextureUnit
    {
        eTU_DiffuseMap	= 0,
        eTU_NormalMap	= 1,
        eTU_SpecularMap	= 2,

        eTU_Count
    };

    //opengl::texture_unit<eTU_DiffuseMap>::activate_and_bind(pTexture, pHighQualitySampler);
    opengl::texture_unit<eTU_NormalMap>::activate_and_bind(pNormal, pHighQualitySampler);
    opengl::texture_unit<eTU_SpecularMap>::activate_and_bind(pSpecular, pHighQualitySampler);
    /*
    simpleShader.program().begin();
    simpleShader.program().sendUniform("in_World", math::matrix4f::identity);
    simpleShader.program().sendUniform("in_DiffuseSampler", int(eTU_DiffuseMap));
    simpleShader.program().sendUniform("in_NormalSampler", int(eTU_NormalMap));
    simpleShader.program().sendUniform("in_SpecularSampler", int(eTU_SpecularMap));
    simpleShader.program().end();
    pMesh->init(simpleShader.program());
    */
    float currentAngle = 0.0f;
    float targetAngle = 0.0f;
    float maxAngle = 30.0f;


    commands["quit"]		= utils::make_command(pDevice, &system::device::shutDown);
    commands["exit"]		= commands["quit"];
    commands["set_resolution"] = utils::make_command<bool, system::device, system::device_sptr, int, int, bool>(pDevice, &system::device::setResolution);

    std::thread t1(readInputs);

    std::vector<directory_changed_handler> directoryListeners(2);
    directoryListeners[0].lpDir_ = TEXT("D:\\Development\\Projects\\djah\\binaries\\assets\\worlds");
    directoryListeners[0].callback_ = [](){ needToReloadWorld = true; };

    directoryListeners[1].lpDir_ = TEXT("D:\\Development\\Projects\\djah\\binaries\\assets\\meshes");
    directoryListeners[1].callback_ = [](){ needToRefreshMeshes = true; };

    directoryListeners[1].lpDir_ = TEXT("D:\\Development\\Projects\\djah\\binaries\\assets\\meshes");
    directoryListeners[1].callback_ = [](){ needToRefreshTextures = true; };

    std::thread t2([&directoryListeners]() { WatchDirectory( directoryListeners ); });

    // 4 - Run app
    while( pDevice->run() )
    {
        if( needToReloadWorld )
        {
            world.load(renderingProcess);
            needToReloadWorld = false;
        }

        if( needToRefreshMeshes )
        {
            resources::default_asset_finder::get().refresh<resources::mesh>();
            world.load(renderingProcess);
            needToRefreshMeshes = false;
        }

        if( needToRefreshTextures )
        {
            resources::default_asset_finder::get().refresh<resources::image>();
            world.load(renderingProcess);
            needToRefreshTextures = false;
        }

        mouse.update();
        kb.update();
        gp.update();

        actionsProcess.execute(0.0f);

        if( pDevice->hasFocus() )
        {
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
                /**/
                gameplay::component_serializer<gpc::DefaultComponentTypes>::deserialize(camera);

                const math::vector3f &realDirection   = math::rotate(cameraTransform->orientation, neutralDirection);
                const math::vector3f &realUpDirection = math::rotate(cameraTransform->orientation, upDirection);
                const math::vector3f &cameraCenter    = cameraTransform->position + realDirection;
                const float aspectRatio = float(std::max(pDeviceConfig->width, pDeviceConfig->height))
                                        / float(std::min(pDeviceConfig->width, pDeviceConfig->height));
                v = math::make_look_at(cameraTransform->position, cameraCenter, realUpDirection);
                p = math::make_perspective_projection
                (
                    math::degree(cameraFOV->horizontal),
                    cameraFOV->distortion * aspectRatio,
                    cameraFOV->nearPlane, cameraFOV->farPlane
                );
                vp = v*p;

                pRendererConfig = resources::open_config<renderer_cfg>("renderer.config");
                const math::vector3f &cc = pRendererConfig->clearColor;
                glClearColor(cc[0], cc[1], cc[2], 1.0f);
                /**/
            }

            if( kb.pressed(system::input::eKC_P) )
            {
                static bool wireframe = false;
                wireframe = !wireframe;
                glPolygonMode(GL_FRONT_AND_BACK , wireframe ? GL_LINE : GL_FILL);
            }

            if( kb.pressed(system::input::eKC_L) )
            {
                //renderingProcess.setSampler(pLowQualitySampler);
                opengl::texture_unit<eTU_SpecularMap>::activate_and_bind(pSpecular, pLowQualitySampler);
            }

            if( kb.pressed(system::input::eKC_H) )
            {
                //renderingProcess.setSampler(pHighQualitySampler);
                opengl::texture_unit<eTU_SpecularMap>::activate_and_bind(pSpecular, pHighQualitySampler);
            }

            if( kb.pressed(system::input::eKC_C) )
            {
                //renderingProcess.setSampler(pCustomSampler);
                opengl::texture_unit<eTU_SpecularMap>::activate_and_bind(pSpecular, pCustomSampler);
            }

            if( resolutionChanged )
            {
                const float aspectRatio = float(std::max(pDeviceConfig->width, pDeviceConfig->height))
                                        / float(std::min(pDeviceConfig->width, pDeviceConfig->height));

                p = math::make_perspective_projection
                (
                    math::degree(cameraFOV->horizontal),
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
                const float camSpeed = 0.5f;
                const math::vector3f &disp = camSpeed * math::vector3f
                (
                    cameraAM->states["MOVE_LEFT"]	  - cameraAM->states["MOVE_RIGHT"],
                    cameraAM->states["MOVE_BACKWARD"] - cameraAM->states["MOVE_FORWARD"],
                    cameraAM->states["MOVE_UPWARD"]	  - cameraAM->states["MOVE_DOWNWARD"]
                );

                bool turned = false;
                const float turnSpeed = 2.7f;

                const float horizontalTurn = cameraAM->states["LOOK_RIGHT"] - cameraAM->states["LOOK_LEFT"];
                if( std::abs(horizontalTurn) > 0.0f )
                {
                    const math::vector3f &realUpDirection = math::rotate(cameraTransform->orientation, upDirection);
                    const math::quatf &nrot = math::make_quaternion(math::degree(-horizontalTurn*turnSpeed), upDirection);
                    cameraTransform->orientation = nrot * cameraTransform->orientation;
                    turned = true;
                }
            
                const float verticalTurn = cameraAM->states["LOOK_UP"] - cameraAM->states["LOOK_DOWN"];
                if( std::abs(verticalTurn) > 0.0f )
                {
                    const math::vector3f realRightDirection(1.0f, 0.0f, 0.0f);
                    const math::quatf &nrot = math::make_quaternion(math::degree(verticalTurn*turnSpeed), realRightDirection);
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
        }

        w = math::make_rotation(math::degree(currentAngle), math::vector3f::z_axis);
        wvp = w*vp;

        pDriver->beginFrame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //cameraTransform->position.y = 1.8f;
        renderingProcess.setMatrixInfos(vp, cameraTransform->position);
        renderingProcess.execute(0.0f);

        glDisable(GL_DEPTH_TEST);
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
        glEnable(GL_DEPTH_TEST);
        
        /**/
        pDriver->endFrame();
    }

    gameplay::component_serializer<gpc::DefaultComponentTypes>::serialize(camera);

    t1.detach();
    t2.detach();
    opengl::texture_unit<eTU_DiffuseMap>::set_active();
    opengl::texture_unit<eTU_DiffuseMap>::unbind();

    opengl::texture_unit<eTU_NormalMap>::set_active();
    opengl::texture_unit<eTU_NormalMap>::unbind();

    opengl::texture_unit<eTU_SpecularMap>::set_active();
    opengl::texture_unit<eTU_SpecularMap>::unbind();

    return EXIT_SUCCESS;
}